/*******************************************************************************
 * Description   :
 *
 *
 * Author :
 *     spasumar@ikanos.com 2009/06/25
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#include "globle_type.h"
#include "reg_landshark.h"
//#include "c_stdio.h"
#include "lib_fs_ubi.h"
#include "stdio.h"
#include "reg_defs.h"
#include "nand_registers.h"
#include "ubi.h"
#include "scu_regs.h"

void init_nand_controller()
{
    /* Update the Timing Control Register based on Tom's measurements */
	if ((reset_strap_val & NAND_RESET_1GB_4KB_MASK) == NAND_RESET_1GB_4KB)
   	{
		/* Modified NAND access timings based on H/W Team's response*/
		*(volatile unsigned long *) 0xb909804c = 0x050f0f0a;
		*(volatile unsigned long *) 0xb9098050 = 0x64140a0a;
   	}
    else
    {
    	/* Modified NAND access timings please Refer Bug#25257*/
    	*(volatile unsigned long *) 0xb909804c=0x04050505;
        *(volatile unsigned long *) 0xb9098050=0x19050505;
    }
}

void stringput(char *s);
void nand_init();

static unsigned int reset_reg_val;
struct nand_info {
	short page_size;  
	unsigned int nand_size;  
	unsigned int block_size;	
	short page_size_shift;
	short oob_size;
	unsigned int nand_ubipart_baseaddr;
	unsigned int num_pages_per_block;
    unsigned int max_offset;
    unsigned int num_sections;
};

struct nand_info nand_chip;

extern short nand_check_page_ecc(int sector, unsigned short page_size, unsigned long mem_addr);
int maptable[20];
int ubootReservedBlocks = 0;

void nand_init(void)
{
    	reset_reg_val = SCU->rst_strp;

	if ((reset_reg_val & NAND_RESET_1GB_4KB_MASK) == NAND_RESET_1GB_4KB)
	{
		nand_chip.page_size 		= NAND_PAGE_SIZE_4KB;
		nand_chip.block_size 		= NAND_BLOCK_SIZE_512KB;
		nand_chip.nand_size 		= NAND_CHIP_SIZE_1GB;
		nand_chip.page_size_shift 	= NAND_PAGE_SHIFT_4KB;
		nand_chip.nand_ubipart_baseaddr = NAND_UBIPART_BASEADDR_512KB;
		nand_chip.oob_size 		    = NAND_OOB_224;
		nand_chip.max_offset 		= NAND_MAX_OFFSET_1GB;
	}
	else
	{
		nand_chip.page_size 		= NAND_PAGE_SIZE_2KB;
		nand_chip.block_size 		= NAND_BLOCK_SIZE_128KB;
		nand_chip.nand_size 		= NAND_CHIP_SIZE_512MB;
		nand_chip.page_size_shift 	= NAND_PAGE_SHIFT_2KB;
		nand_chip.nand_ubipart_baseaddr = NAND_UBIPART_BASEADDR_128KB;
		nand_chip.oob_size 		= NAND_OOB_64;
		nand_chip.max_offset 		= NAND_MAX_OFFSET_512MB;
	}
	nand_chip.num_pages_per_block = nand_chip.block_size/nand_chip.page_size;
	nand_chip.num_sections = nand_chip.page_size/512;
}

/*******************************************************************************
  * Function Name     : read_nand_page
  * Input Paramaters  : dest - buffer address to store read page
            src  - NAND offset to read from
            len - no. of bytes to read 
  * Output Paramaters : None
  * Result            : 
  * Description       : This function read 'len' bytes from 'src' offset of NAND 
  *         flash to 'dest' buffer 
*******************************************************************************/
void read_nand_page(unsigned int dest, unsigned int src, unsigned int len)
{
    volatile unsigned int output, i;
    volatile unsigned int j;

    //c_put_str("NAND:");
    EP501M3_WR(EP501M3_COMMAND, 0x00);
    EP501M3_WR(EP501M3_INDEX, src);
    EP501M3_WR(EP501M3_COMMAND, 0xE0);
    //c_put_char('1');

    /* wait for chip_busy to go low */
    polling_until_set(H_RD32(0xb90000bc), 1);
    //c_put_char('2');

    /* Read the data */
    for (i = 0; i < len; i+=4)
    {
        H_WR(dest + i) = EP501M3_RD(EP501M3_DATA + i);
    }
    //c_put_char('3');

   	if(!(EP501M3_RDREG(EP501M3_INDEX) & 0x800))
   	{   
		i=20;

      	/* start creating ECC */
       	EP501M3_WRREG(EP501M3_COMMAND, 0x23);

		/* PECC_BUSY takes a few clocks to assert */
       		while (!(SCU->nand_status & NAND_PECC_BUSY) && i--);

        	/* Wait for PECC_BUSY to go low */
        	while (SCU->nand_status & NAND_PECC_BUSY);
   	}

	EP501M3_WRREG(EP501M3_COMMAND, 0xEE);

#if 0
    /* start creating ECC */
    EP501M3_WR(EP501M3_COMMAND, 0x23);
    for (i = 0; i < 10; i++)
        j = i;
    //c_put_char('4');
    /* wait till ECC is busy */
    do {
        output = *(volatile unsigned int *)0xb90000bc;
    } while ((output & 0x2) == 0x2);
    //c_put_char('5');

    /* read ecc status */
    output = EP501M3_RD(EP501M3_BCH_STATUS);
    EP501M3_WR(EP501M3_COMMAND, 0xEE);
    //c_put_char('\r');c_put_char('\n');
#endif
}
void convertInt_Text(unsigned int address, char *outASC)
{
    int i, j;
    char   b;
   
    outASC[8] = '\0';
    for(i = 0, j = 0; i < 8; i++)
    {
        b = (address >> j) & 0x0f;
        if(b >= 10)
            outASC[7-i] = b - 10 + 'a';
        else
            outASC[7-i] = b + '0';
        j += 4;
    }
}

#if defined(UARTBOOT_CODE)

int read_uboot(void)
{
    volatile unsigned int i, j;

    /* Modified NAND access timings please Refer Bug#25257*/
    EP501M3_WR(EP501M3_TIMING_0, 0x04050505);
    EP501M3_WR(EP501M3_TIMING_1, 0x19050505);

    /* read u-boot (256 KB block) */
    for ( i = 0; i < 128; i++)
    {
        read_nand_page((unsigned int)(0xA0800000 + (i * 2048)), (unsigned int)(0x80000 + (i * 4096)));
        for(j = 0; j < 10000; j++);
    }
    c_put_char('\d');
    return 0;
}

#else

/*******************************************************************************
  * Function Name     : volname_compare
  * Input Paramaters  : char *s : volume name read by scanlayoutv routine
            len : length of volume name 
  * Output Paramaters : None
  * Result            : '0' if a match is found. returns '1' otherwise
  * Description       : This function is called during layout table scan
  *         to identify if the record belongs to 'uboot' volume'.  
  *         The function compares the volume name read from layout 
  *         volume with 'uboot'
*******************************************************************************/
int volname_compare( char *s, int len, int ubootVol )
{
    int count;
    const char *s1= "uboot";
    const char *s2= "BKUPuboot";
    char* compare;
    char *msgBuf;

    if(!ubootVol)
        compare = s1;
    else
        compare = s2;

    for(count = 0; count < len; count++)
    {
        if ( s[count] != compare[count] )
        {
#ifdef DEBUG_FBOOT
            c_put_str("Volume Name Mismatch");
#endif
            return 1;
        }
    }

    if( s[count]=='\0' && compare[count] == '\0' )
        return 0;
    else
    {
#ifdef DEBUG_FBOOT
        msgBuf = "Volume Name Mismatch(String Termination)\n";
        c_put_str(msgBuf);
#endif
        return 1;
    }
}

/*******************************************************************************
  * Function Name     : check_pattern
  * Input Paramaters  : @buf: buffer to check
  *             @patt: the pattern to check
  *             @size: buffer size in bytes
  * Output Paramaters : None
  * Result            : returns 1 if there are only @patt bytes in @buf, 
  *                     returns 0 if something else was also found.
  * Description       : check if buffer contains only a certain byte pattern. 
  *         
*******************************************************************************/
static int check_pattern(const void *buf, char patt, int size)
{
    int i;

    for (i = 0; i < size; i++)
        if (((const char *)buf)[i] != patt)
            return 0;
	
    return 1;
}

/**************************************************************************
 * Function Name 	: checkErasedPage 				  *
 * Input Parameters 	: @offset : page offset				  *
 * Output Paramerts 	: None						  *
 * Result 	   	: returns 1 if the page is erased		  *	
 *			  returns 0 if the page is programmed 		  *
 * Description		: Check if the page is erased.       		  *
 *                                                                        *
 * 1. If 32 bytes of oob area are 0xFF, consider the page as erased.      *
 *    However bit-flips during oob area area can cause this logic to fail *
 * 2. Check for byte 4 & 5 of the ecc for each section. When a page is    *
 *    programmed 12 bits of those 2 bytes are expected to be '0'.         * 
 *    Considering 4 possible bit flips, atleast 8 bits have to be '0' to  *
 *    flag a page as 'programmed'                                         *
 *                                                                        *     
 **************************************************************************/
int checkErasedPage(unsigned int offset)
{
#ifdef ECC_ENABLE
	unsigned int page;
	unsigned char *sectionEcc;
	unsigned int i, clean,section,zeroBits=0;
	unsigned short ecc45 =0;
	
    	page = ( (offset + nand_chip.page_size) >> (nand_chip.page_size_shift-1)); //Read oob. Hence, 'offset+ pagesize'.
    	read_nand_page((unsigned int)DDR_BUFFER_ADDR , ((page << nand_chip.page_size_shift)+ nand_chip.page_size) , nand_chip.oob_size); 

	if(check_pattern((unsigned char *) DDR_BUFFER_ADDR, 0xFF, nand_chip.oob_size))
		return 1;
	else
	{
		 clean = 1;
		 // maybee a bit (or a few bits) have flipped
                for (section = 0; section < nand_chip.num_sections; section++)
                {
			sectionEcc = (unsigned char *)(DDR_BUFFER2_ADDR + 32 + 8 *section);
                        ecc45 = (sectionEcc[NAND_ECC_BYTE4] << 4) | (sectionEcc[NAND_ECC_BYTE5] >> 4);
                        for (i = 0; i < NAND_MAX_COUNT_ZERO; i++)
                                 if((ecc45 & (1 << i)) == 0) zeroBits++;
                        if (zeroBits >= NAND_MIN_COUNT_ZERO)
                        {
                                clean = 0;
                                break;
                        }
                }
                if (clean) return 1; 
	}
	return 0;
#else
	return 0;
#endif
}

/*******************************************************************************
  * Function Name     : checkBadBlock
  * Input Paramaters  : Block Offset
  * Output Paramaters : None
  * Result            : '1' on detection of Bad Block, returns '0' otherwise
  * Description       : This function scans for the any Bad blocks by checking the first 
  *			byte of the first page of each block. If the value read is not 0xFF,
  *			the block is identified as Bad Block
*******************************************************************************/
int checkBadBlock(unsigned int offset)
{
    unsigned int page;
    unsigned int index;
    volatile unsigned int j;

    	page = ( (offset + nand_chip.page_size) >> (nand_chip.page_size_shift-1)); //Read oob. Hence, 'offset+pagesize'.
    	read_nand_page((unsigned int)DDR_BUFFER_ADDR , ((page << nand_chip.page_size_shift ) + nand_chip.page_size),1); //checking for BAD Block
    	if( *(unsigned char *)DDR_BUFFER_ADDR !=0xFF)
    {
        c_put_char('B');
        c_put_char(':');
        printHex_u32(offset);
        c_put_char('\r');c_put_char('\n');
        for(index=0;index<0xfffff;index++) //Delay slot
            j = index;
        return 1;
    }
    return 0;
}

/*******************************************************************************
  * Function Name     : scanlayoutv
  * Input Paramaters  : None
  * Output Paramaters : None
  * Result            : 
  * Description       : This function scans for the Layout volume. On finding the layout 
  *			volume, the function finds the volume id for the 'uboot' partition. 
  *			This volumeid is later used by scanvid for creating the maptable.
  *			The function also skips any bad blocks detected during the scan. 
  *			crc32 verification is performed to verify the correctness of each 
  *			volume table record.  
*******************************************************************************/
int scanlayoutv( int ubootVol )
{
    int numLayoutV=0,vol_id=0;
    int offset, page, crc;
    struct ubi_vtbl_record *vtbl_record;
    volatile unsigned int errs=0;
    int section=0;
    char *msgBuf;

    /* Layout Volume Scan */
    	for ( offset = nand_chip.nand_ubipart_baseaddr; offset < nand_chip.max_offset; 
			offset=(offset + nand_chip.block_size) )
    {
        if ( checkBadBlock(offset) )
            continue;   
		if ((reset_reg_val & NAND_RESET_1GB_4KB_MASK) != NAND_RESET_1GB_4KB)
		{
			//Skip any empty pages found
	                errs = checkErasedPage(offset);
			if (errs)
				continue;
        //EC in Page 0, VID (layout Volume) in Page 1   
	        	page = ( offset + nand_chip.page_size ) >> (nand_chip.page_size_shift-1); 
        		read_nand_page( (unsigned int)DDR_BUFFER_ADDR ,(page << nand_chip.page_size_shift ), nand_chip.page_size );
		}
		else
		{
			//EC in Page 0, VID (layout Volume) in Page 1	
	        	page = ( offset + nand_chip.page_size ) >> (nand_chip.page_size_shift-1); 
		        read_nand_page( (unsigned int)DDR_BUFFER_ADDR , 
				(page <<nand_chip.page_size_shift ), nand_chip.page_size );
        //Skip any empty pages found
                	errs = check_pattern( (unsigned char*)DDR_BUFFER_ADDR,0xFF, nand_chip.page_size );
        if ( errs )
            continue;
		}

#ifdef ECC_ENABLE 
        /* run ECC algorithm on section 0 of the page read */
        	errs = nand_check_page_ecc( 0, nand_chip.page_size, (DDR_BUFFER_ADDR) );
        if ( errs < 0 )
        {
            msgBuf = "ECC Error during Layout Volume Scan 1.\0";
            c_put_str(msgBuf);
            continue;   
        }
#endif
        /* Identify layout Volume */
        if ( *(volatile unsigned int *)(DDR_BUFFER_ADDR + VOLUMEID_OFFSET) == LAYOUT_VOLUMEID )
        {
			if ((reset_reg_val & NAND_RESET_1GB_4KB_MASK) != NAND_RESET_1GB_4KB)
			{
	    			//Skip any empty pages found
	                	errs = checkErasedPage(offset);
				if (errs)
					continue;
            /* Data comes in Page2(offset 0x1000). 
               Page0-EC header, 
               Page1-VID Header
            */
	    			page = ( offset + (nand_chip.page_size * 2) ) >> (nand_chip.page_size_shift-1); 
	            		read_nand_page( (unsigned int)DDR_BUFFER_ADDR , (page << nand_chip.page_size_shift), nand_chip.page_size );
			}
			else
			{
			        /* Data comes in Page2(offset 0x1000). 
                        	   Page0-EC header, 
	                           Page1-VID Header
        	                */
                  	   	page = ( offset + (nand_chip.page_size * 2) )>> (nand_chip.page_size_shift-1);
                        	read_nand_page( (unsigned int)DDR_BUFFER_ADDR , (page << nand_chip.page_size_shift ), nand_chip.page_size );

            //Skip any empty pages found
	                        errs = check_pattern( (unsigned char*)DDR_BUFFER_ADDR,0xFF, nand_chip.page_size );
            if ( errs )
                continue;
			}
#ifdef ECC_ENABLE
            /* ECC Check */
            		for ( section = 0; section < nand_chip.num_sections; section++ )
            {
                		errs = nand_check_page_ecc( section, nand_chip.page_size, 
							    (DDR_BUFFER_ADDR) );
                if ( errs < 0 )
                {
                    msgBuf = "ECC Error during Layout Volume Scan 2.\0";
                    c_put_str(msgBuf);
                }
            }
#endif
            vtbl_record = (struct ubi_vtbl_record *) DDR_BUFFER_ADDR;
            		do
            		{
                		if ( volname_compare( vtbl_record->name, 
							vtbl_record->name_len, ubootVol ) == 0)
                {
                    //Volume table record CRC check 
	    				crc = crcFast( (unsigned char *)vtbl_record, UBI_VTBL_RECORD_SIZE_CRC);
                    /* check for Valid Volume table header & Page ECC*/
                    if ( (crc == vtbl_record->crc) && (errs >= 0) )
                    {
                        //check if an update was interrupted.
                        if(!vtbl_record->upd_marker) 
                        {
                            /* Read Number of Reserved Blocks for 'uboot' volume */
                            ubootReservedBlocks = vtbl_record->reserved_pebs;
                            numLayoutV++;
                            break;
                        }
                        else //corrupted page. check for back-up layout volume
                            break; 
                    }
                    /*Header CRC Mismatch.Skip this Layout volume & Check for the backup Layout Volume */
                    else
                        break;
                }
                else //Doesn't match uboot vol_id. Advance to next record
                    vtbl_record++; 
                } while(vol_id++ < UBI_MAX_VOLUMES);
        }
        //After finding a proper layout volume(not corrupted), break out of layout table scan
        if(numLayoutV)
            return vol_id;
        }
        return -1;
}

/*******************************************************************************
  * Function Name     : scanVid
  * Input Paramaters  : volume id  
  * Output Paramaters : None
  * Result            : 
  * Description       : The function scans the volumeID header in each block & 
  *             checks if the vol_id matches with the volume id of uboot  
  *         . If vol_id matches, map table is updated with the 
  *         offset of PEB. Function also skips any Bad Blocks that it 
  *         finds during the scan.  VID header is verified for any errors 
  *         with crc32 function call after each read. 
*******************************************************************************/
int scanvid(int vol_id)
{
    unsigned int offset,page,crc;
    struct ubi_vid_hdr *vidhdr;
    volatile int errs=0;
    int section,index;
    volatile unsigned int j=0;
    unsigned int ubootblocks =0;
    int err1;
    char *msgBuf;

    /* Scan VID Header & create MAP table ( LEB -> PEB) */
    /* Clear Map table.This table will later be populated with PEB number. Array index corresponds to the LEB number */
    for(index =0; index < 20; index++)
        maptable[index] =0;

    	for(offset = nand_chip.nand_ubipart_baseaddr ; offset < nand_chip.max_offset; offset=(offset + nand_chip.block_size))
    {
        if(checkBadBlock(offset))
            continue;   

		if ((reset_reg_val & NAND_RESET_1GB_4KB_MASK) != NAND_RESET_1GB_4KB)
		{
		    	//Skip any empty pages found
			errs = checkErasedPage(offset);
			if ( errs )
			    continue;
        		/* Offset points to the start of Block
           		   EC Header resides in page 0 & VID header in i
			   Page 1. Hence read only page 1 
			*/
        		page = ((offset + nand_chip.page_size) >> (nand_chip.page_size_shift-1));
	        	read_nand_page( (unsigned int)DDR_BUFFER_ADDR, (page << nand_chip.page_size_shift),nand_chip.page_size);
		}
		else
		{
			/* Offset points to the start of Block
	                   EC Header resides in page 0 & VID header in i
        	           Page 1. Hence read only page 1 
                	*/
	                page = ((offset + nand_chip.page_size) >> (nand_chip.page_size_shift-1));
	                read_nand_page( (unsigned int)DDR_BUFFER_ADDR,(page << nand_chip.page_size_shift), nand_chip.page_size);
        //Skip any empty pages found
	                errs = check_pattern((unsigned char*)DDR_BUFFER_ADDR, 0xFF, nand_chip.page_size);
        if(errs)
            continue;
		}
#ifdef ECC_ENABLE
        //run ECC algorithm on the page read
       		errs = nand_check_page_ecc(0, nand_chip.page_size, (DDR_BUFFER_ADDR));
        if (errs < 0) 
        {
            msgBuf = "ECC Error during VID Scan 1.\0";
            c_put_str(msgBuf);
            continue;
        }
#endif
        if( *(volatile unsigned int *)(DDR_BUFFER_ADDR) == UBI_VID_HDR_MAGIC) //VID MAJIC
        {
#ifdef ECC_ENABLE
            //run ECC algorithm on the page read
	    		for (section = 1; section < nand_chip.num_sections; section++)
            {
                		errs = nand_check_page_ecc( section, nand_chip.page_size, 
						            (DDR_BUFFER_ADDR));
                if(errs < 0)
                {
                    msgBuf = "ECC Error during VID Scan 2.\0";
                    c_put_str(msgBuf);
                    continue;
                }
            }
#endif
            vidhdr = (struct ubi_vid_hdr*)DDR_BUFFER_ADDR;
            if(vidhdr->vol_id == vol_id) //Match the Volume ID read from Volume Table
            {
                //header CRC check  
				crc = crcFast((unsigned char *)vidhdr, UBI_VID_HDR_SIZE_CRC);
                if((crc == vidhdr->hdr_crc) && (errs >= 0))
                {
                    //Data starts at page3
                        		maptable[vidhdr->lnum] = ( offset + (nand_chip.page_size * 2) );
                    ubootblocks++;
                    /* Uboot should not take up more than 
                       ubootReservedBlocks(0x3 blocks) as 
                       per layout volume. Return on reading all blocks*/
                    if(ubootblocks >= ubootReservedBlocks) 
                        return 0;
                }
                else /* CRC ERROR */
                {
                    msgBuf = "Header CRC Error -VID Scan.\0";
                    c_put_str(msgBuf);
                    return -1;
                }
            }       
        }
    }
    if(ubootblocks < ubootReservedBlocks)
    {
        msgBuf = "Could not Find Uboot Blocks.Error!!\0";
        c_put_str(msgBuf);
        return -1;
    }
    return 0;
}

/*******************************************************************************
  * Function Name     : readmapTable
  * Input Paramaters  : None 
  * Output Paramaters : None
  * Result            : 
  * Description       : This function Reads the Map table to find the PEB where uboot is located
  *             It then reads from the respective PEBs & loads uboot
  *
*******************************************************************************/
int readmapTable(void)
{
	unsigned int index,bytes;
	unsigned int page=0;
	int section =0;
    volatile int errs=0;
	int i;
    char *msgBuf;

#if DEBUG_FBOOT
    /* Print mapTable (PEB-LEB map) */
    index =0;
    do {
        printHex_u32(maptable[index++]);
        c_put_char('\r'); c_put_char('\n');
    } while(maptable[index] !=0);
#endif
    /* Try booting the Uboot reading the PEBs from maptable */
    bytes =0;
    index = 0;
    	do
    	{
		/* 64 pages per block, 2 pages for headers. 
		   Data in 62 pages */
        	for(i=0; i< (nand_chip.num_pages_per_block - 2); i++) 
        {
			if ((reset_reg_val & NAND_RESET_1GB_4KB_MASK) != NAND_RESET_1GB_4KB)	
			{
            			page = ((maptable[index] + (i * nand_chip.page_size)) >> (nand_chip.page_size_shift-1));
	            		read_nand_page( ((unsigned int)UBOOT_TEXT_BASE + bytes), 
					(page << nand_chip.page_size_shift), nand_chip.page_size);

				//Skip any empty pages found
                		errs = checkErasedPage(maptable[index] + (i * nand_chip.page_size));
                        	//Reached End of Uboot. Hence, return
	                        if(errs)
        	                        return 1;
			}
			else
			{
			       page = ( (maptable[index] + (i * nand_chip.page_size)) >> (nand_chip.page_size_shift-1));
	                       read_nand_page( ((unsigned int)UBOOT_TEXT_BASE + bytes),
                                        (page << nand_chip.page_size_shift), nand_chip.page_size);
                
	                        //Skip any empty pages found
        	                errs = check_pattern( ((unsigned char*)UBOOT_TEXT_BASE + bytes),
                                                0xFF, nand_chip.page_size );
                	        //Reached End of Uboot. Hence, return
            if(errs)
                return 1;
			}

#ifdef ECC_ENABLE
            /* ECC Check */
            		for (section = 0; section < nand_chip.num_sections ; section++)
            {
                		errs = nand_check_page_ecc( section, nand_chip.page_size, 
						((unsigned int)UBOOT_TEXT_BASE + bytes) );
                if(errs < 0)
                {
                    msgBuf = "ECC Error. Read Map Table!!\0";
                    c_put_str(msgBuf);
                    return -1;
                }
            }
#endif
            		bytes += nand_chip.page_size; //Total bytes read. each read of 2048 bytes
        }
        index++;
    } while (maptable[index] != 0);
    return 0;
}

/*******************************************************************************
  * Function Name     : read_uboot
  * Input Paramaters  : None 
  * Output Paramaters : None
  * Result            : 
  * Description       : This function performs the following tasks
  *    1. scans layout volume to find the volume id of 'uboot' volume
  *    2. scans vid header of the entire flash to find PEBs for 'uboot' volume
  *    3. creates a map table with PEB offsets
  *    4. Reads map table & boots uboot 
  *    5. Switch to uboot backup volume for an error returned by 
  *       any of the steps above.
*******************************************************************************/
int read_uboot(void)
{
    int vol_id=0;
    int ubootVol=0;
    char *msgBuf;

	nand_init();

    crcInit();

    for( ubootVol = 0; ubootVol < MAX_VOLUMES_UBOOT; ubootVol++)
    {
        vol_id = scanlayoutv( ubootVol );
        /* Check for Backup Uboot Volume in case of error */
        if( vol_id < 0 )
        {
            if( ubootVol )
                return -1;
            else
            {
                msgBuf = "Uboot layout Scan failed.Switching to Backup Volume\0";
                c_put_str(msgBuf);
                c_put_char('\r');c_put_char('\n');
                continue;
            }
        }

        /* Check for Backup Uboot Volume in case of error */
        if( scanvid( vol_id ) < 0 )
        {
            if( ubootVol )
                return -1;
            else
            {
                msgBuf = "Uboot Volume ID failed. Switching to Backup Volume\n";
                c_put_str(msgBuf);
                continue;
            }
        }
    
        msgBuf = "STAGE2.\n";
        c_put_str(msgBuf);

        if(readmapTable() < 0)//Check Backup Uboot Volume in case of error 
        {
            if( ubootVol )
                return -1;
            else
            {
                c_put_char('B');
                c_put_char('3');
                msgBuf = "EAB Table Read failed. Switching to Backup Volume\n";
                c_put_str(msgBuf);
                continue;
            }
        }

        if(!ubootVol)
            msgBuf = "Booting from Primary Uboot Volume.\n";
        else
            msgBuf = "Error in Primary Uboot Volume!!Booting from Backup Uboot Volume\n";
        c_put_str(msgBuf);

        return 0;
    }
}

void dummy_func(void)
{
    return;
}

short nand_check_page_ecc(int sector, unsigned short page_size, unsigned long mem_addr) 
{
    volatile int errs = 0;

    __asm__ __volatile__ (
        ".set mips16;"
        "jal 0x991d20e4;"  /* Address of correct_page function in LMEM */
        "nop;"
        "nop;"
        "nop;"
    );

    /* Store the return value (from v0) in local variable, errs */
    __asm__ __volatile__ (
        "move %1, $v0\n"\
        :"=r"(errs)\
        :"0"(0)
    );
  
    dummy_func();
    return errs;
}

#endif
