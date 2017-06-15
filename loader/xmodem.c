/*******************************************************************************
*                                                                        
*                             Vx185 Bootrom
*              Copyright (c) Ikanos Communications Inc. 2009              
*                                                                        
*   All rights are reserved.  Reproduction in whole or in part is prohibited
*   without the written consent of the copyright owner.
*
*   The information and contents of this file are the proprietary information
*   of Ikanos Communication and may not be disclosed or used without the
*   formal written approval of Ikanos Communication Inc.
*
*   This Copyright notice may not be removed or modified without prior
*   written consent of Ikanos Communications, Inc.
*
*******************************************************************************/
/******************************************************************************
* Performs a serial boot using XMODEM.
* ------------------------------------
*
* Set up UART0 as an XMODEM receiver operating at BOOT_PRIMARY_BAUDRATE baud,
* no parity, 8 data bits and one stop bit.  A backup baud rate of 
* BOOT_SECONDARY_BAUDRATE is also available.  Sending a BREAK on the serial
* line will toggle between these two rates.  The dual rate is supported
* because the low frequency of the UART at boot (12.5MHz) means the baud
* rates are not too accurate (57600 baud has 3% error, 38400 baud has 1.7%
* error and 9600 baud has 0.5% error).  The primary rate is a balance between
* a moderate baud rate error and speed; the secondary rate is slower, but
* more accurate should the primary rate be too unreliable.
*
* The code will place the blocks in local SRAM (LMEM) as they arrive.
* The block number will determine where in the LMEM the block is placed:
* block 0 is placed at the start of LMEM, block 1 at the start plus 128 bytes
* and so on.  As the LMEM is only 16kB in size, the maximum possible block
* number is 128 and the code does not expect or handle higher block numbers.
* Note that as the bootrom uses some of the top of LMEM for itself, the maximum
* block number is less than 128 in practice.
*
* Once the download is complete, the program is run by jumping to the start
* of LMEM in MIPS32 mode.
*
*******************************************************************************/
#include "global_type.h"
#include "reg_landshark.h"
//#include "bootrom.h"
#include "uart_boot.h"
#include "timer_boot.h"
#include "soc_boot.h"

#define ASCII_SOH   (0x01)
#define ASCII_EOT   (0x04)
#define ASCII_ACK   (0x06)
#define ASCII_NAK   (0x15)

#define GETCHAR_TIMED_OUT -1
#define RX_BUF_SIZE 130

/* Positions in receive buffer of the block numbers and where the data starts */
#define BLK_NUM 0
#define INV_BLK_NUM 1
#define DATA_START 2

unsigned long xmodem_load(unsigned long download_address)
{
    int ch, i;
    unsigned int expected_blk_num, blk_num;
    unsigned char rx_buf[RX_BUF_SIZE];
    unsigned char *pmem;
    unsigned char crc;
    unsigned long one_second, ten_seconds;
    unsigned int mem_index;
    enum { WaitForNewPacket, ReceivePacket, Purge, SendNak } state;
    
    /* Ensure CP0 COUNT is running, we use it for timeouts */
    //enable_mips_cp0_count();
    
    /* Clear any pending input characters */
    while (c_get_char_timout(0) != GETCHAR_TIMED_OUT);
    
    /* COUNT register increments at core_clock / 2 */
    one_second = gSysClock;
    ten_seconds = 10 * one_second;

    state = Purge;
    expected_blk_num = 1;
    pmem = (unsigned char *)download_address;
    mem_index = 0;
    
    while (1) {
        switch(state) {
        case WaitForNewPacket:
            /* Waiting after successful packet completion.  The
            *  sender can send <EOT> to signal the download is
            *  complete.  If it does, we run the downloaded
            *  application.  Otherwise we expect the start of
            *  the next packet with an <SOH>.
            */
            ch = c_get_char_timout(ten_seconds);
            if (ch == GETCHAR_TIMED_OUT) {
                state = SendNak;
            } else if (ch == ASCII_SOH) {
                state = ReceivePacket;
            } else if (ch == ASCII_EOT) {
                /* All packets received, return to monitor */
                c_put_char(ASCII_ACK);
                return mem_index;
            } else {
                state = Purge;
            }
            break;
        case ReceivePacket:
            /* Read in the remaining 131 byes of the packet after
            *  the initial <SOH>.  The data should be the block
            *  number, the bitwise inverse of the block number,
            *  128 bytes of data, and the 8 bit arithmetic CRC.
            */
            crc = 0;
            for (i = 0; i < RX_BUF_SIZE + 1; i++) {
                ch = c_get_char_timout(one_second);
                if (ch == GETCHAR_TIMED_OUT) {
                    state = SendNak;
                    break;
                }
                /* Do not store the final CRC byte in the
                *  receive buffer, just pass it on in 'ch'.
                */
                if (i < RX_BUF_SIZE) {
                    rx_buf[i] = (unsigned char)ch;
                    /* Only CRC data bytes */
                    if (i > 1) {
                        crc += ch;
                    }
                }                   
            }
            
            /* Check for a valid CRC and block number */
            blk_num = (unsigned)rx_buf[BLK_NUM];
            if (((unsigned)crc != ch) || ((blk_num ^ (unsigned)rx_buf[INV_BLK_NUM]) != 0xff))
            {
                /* Bad CRC or block number packet received */
                state = Purge;
                break;
            }
            
            /* Valid packet received, copy it to memory.  If the
            *  sender missed our ACK then it will resend the 
            *  same packet and we ack it again (but don't copy it).
            */
            if (blk_num == expected_blk_num) {
                for (i = DATA_START; i < RX_BUF_SIZE; ) {
                    pmem[mem_index++] = rx_buf[i++];
                }
                c_put_char(ASCII_ACK);
                expected_blk_num = ((blk_num + 1) & 0xff);
            } else if (blk_num == ((expected_blk_num - 1) & 0xff)) {
                /* Sender missed our ACK of the last packet */
                c_put_char(ASCII_ACK);
            } else {
                /* Block number out of sequence */
                state = Purge;
                break;
            }
                
            state = WaitForNewPacket;
            break;
        case Purge:
            /* Something's gone wrong - wait until transmitter
            *  pauses (indicating the end of any packet being sent)
            *  before proceeding to NAK the packet.
            *  Also start here to wait 1 second before sending the
            *  first NAK that tells the transmitter we are ready.
            */
            do {
                ch = c_get_char_timout(one_second);
            } while (ch != GETCHAR_TIMED_OUT);
            /* deliberate drop through to SendNak state */
        case SendNak:
            c_put_char(ASCII_NAK);
            /* deliberate drop through to default state */
        default:
            state = WaitForNewPacket;
            break;
        }
    }
}

