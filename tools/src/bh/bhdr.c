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
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define NAND_MAGIC 0x4c424348

struct nand_header {
    unsigned char magic_3;
    unsigned char magic_2;
    unsigned char magic_1;
    unsigned char magic_0;
    unsigned char bootstrap_len_3;
    unsigned char bootstrap_len_2;
    unsigned char bootstrap_len_1;
    unsigned char bootstrap_len_0;
    unsigned char timing0_3;
    unsigned char timing0_2;
    unsigned char timing0_1;
    unsigned char timing0_0;
    unsigned char timing1_3;
    unsigned char timing1_2;
    unsigned char timing1_1;
    unsigned char timing1_0;
    unsigned char bootstrap_crc_3;
    unsigned char bootstrap_crc_2;
    unsigned char bootstrap_crc_1;
    unsigned char bootstrap_crc_0;
    unsigned char header_crc_3;
    unsigned char header_crc_2;
    unsigned char header_crc_1;
    unsigned char header_crc_0;
} __attribute__ ((packed));

#define SPI_BOOT_SIG (unsigned char)0x4d
struct spi_header {
    unsigned char spi_boot_sig1;
    unsigned char proglen_2;
    unsigned char proglen_1;
    unsigned char proglen_0;
    unsigned char progld_3;
    unsigned char progld_2;
    unsigned char progld_1;
    unsigned char progld_0;
    unsigned char progcrc_3;
    unsigned char progcrc_2;
    unsigned char progcrc_1;
    unsigned char progcrc_0;
    unsigned char hdrcrc_3;
    unsigned char hdrcrc_2;
    unsigned char hdrcrc_1;
    unsigned char hdrcrc_0;
} __attribute__ ((packed));

struct cb {
    int nand;
    union {
        struct nand_header nand_header;
        struct spi_header spi_header;
    } hdr;
    char *binary_file_name;
    char *output_file_name;
};

static unsigned long crc32 = 0;
static char *prog_name = 0;

static unsigned long crc32table[256];

void init_crc_table(void)
{
    unsigned long crc;
    int i, j;
    const unsigned long dwpoly = 0xEDB88320;
    
    for (i = 0; i < 256; i++) {
        crc = i;
        for(j = 8; j > 0; j--) {
            if(crc & 1)
                crc = (crc >> 1) ^ dwpoly;
            else
                crc >>= 1;
        }
        crc32table[i] = crc;
    }
}

void update_crc32(unsigned char data)
{
    crc32 = (crc32 >> 8) ^ crc32table[(data ^ crc32) & 0xff];
}

void print_usage(void)
{
    printf("\nusage: %s options infile outfile\n", prog_name);
    printf("options:\n"
        "\t[ -s [ -l value ] ]\n"
        "\t[ -0 value ] [ -1 value ]\n"
        "\tDefault image is NAND\n"
        "\t-s  -  SPI image (must be first option)\n"
        "\t-l  -  load address (and entry point) of SPI image\n"
        "\t-0  -  timing value 0 for NAND flash images\n"
        "\t-1  -  timing value 1 for NAND flash images\n\n");
}

int parse_cmd_line(struct cb *cb, int argc, char **argv)
{
    int opt, s_opt_set, zero_opt_set, one_opt_set;
    int zero_opt_val, one_opt_val, l_opt_val;

    s_opt_set = 0;
    zero_opt_val = 0;
    one_opt_val = 0;
    zero_opt_set = 0;
    one_opt_set = 0;
    l_opt_val = 0xb91d0000; /* Start of LMEM */

    while ((opt = getopt(argc, argv, "s0:1:l:")) != -1 ) {
        switch (opt) {
        case 's':/* SPI header */
            if (zero_opt_set || one_opt_set) {
                return -1;
            }
            s_opt_set = 1;
            break;
        case 'l':
            {
                if (!s_opt_set) {
                    return -1;
                }
                l_opt_val = strtoul(optarg, 0, 0);
                break;      
            }
        case '0':
            {
                if (s_opt_set) {
                    return -1;
                }
                zero_opt_val = strtoul(optarg, 0, 0);
                zero_opt_set = 1;
                break;      
            }
        case '1':
            {
                if (s_opt_set) {
                    return -1;
                }
                one_opt_val = strtoul(optarg, 0, 0);
                one_opt_set = 1;
                break;
            }
        default:
            break;
        }
    }
    
    /* Source binary file and output file names are expected next. */
    if (optind > argc - 2)
        return -1;
    
    if (s_opt_set) {
        /* SPI header */
        /* Initialise magic */
        cb->hdr.spi_header.spi_boot_sig1 = SPI_BOOT_SIG;
        cb->nand = 0;
        cb->hdr.spi_header.progld_3 = (l_opt_val >> 24) & 0xff;
        cb->hdr.spi_header.progld_2 = (l_opt_val >> 16) & 0xff;
        cb->hdr.spi_header.progld_1 = (l_opt_val >> 8) & 0xff;
        cb->hdr.spi_header.progld_0 = (l_opt_val >> 0) & 0xff;
    } else {
        /* NAND header */
        /* Initialise magic */
        cb->hdr.nand_header.magic_3 = (NAND_MAGIC >> 24) & 0xff;
        cb->hdr.nand_header.magic_2 = (NAND_MAGIC >> 16) & 0xff;
        cb->hdr.nand_header.magic_1 = (NAND_MAGIC >> 8) & 0xff;
        cb->hdr.nand_header.magic_0 = (NAND_MAGIC >> 0) & 0xff;
        cb->hdr.nand_header.timing0_3 = (zero_opt_val >> 24) & 0xff;
        cb->hdr.nand_header.timing0_2 = (zero_opt_val >> 16) & 0xff;
        cb->hdr.nand_header.timing0_1 = (zero_opt_val >> 8) & 0xff;
        cb->hdr.nand_header.timing0_0 = (zero_opt_val >> 0) & 0xff;
        cb->hdr.nand_header.timing1_3 = (one_opt_val >> 24) & 0xff;
        cb->hdr.nand_header.timing1_2 = (one_opt_val >> 16) & 0xff;
        cb->hdr.nand_header.timing1_1 = (one_opt_val >> 8) & 0xff;
        cb->hdr.nand_header.timing1_0 = (one_opt_val >> 0) & 0xff;
        cb->nand = 1;
    }
    
    cb->binary_file_name = strdup(argv[optind]);
    cb->output_file_name = strdup(argv[optind + 1]);
    
    return 0;
}

int get_binary_file_length(struct cb *cb)
{
    struct stat sbuf;
    unsigned long len;
    
    if (stat(cb->binary_file_name, &sbuf) != 0)
        return -1;
    
    if (cb->nand) {
        len = (sbuf.st_size + sizeof(struct nand_header) + 3) >> 2;
        cb->hdr.nand_header.bootstrap_len_3 = (len >> 24);
        cb->hdr.nand_header.bootstrap_len_2 = (len >> 16);
        cb->hdr.nand_header.bootstrap_len_1 = (len >> 8);
        cb->hdr.nand_header.bootstrap_len_0 = (len >> 0);
    } else {
        cb->hdr.spi_header.proglen_2 = (sbuf.st_size >> 16) & 0xff;
        cb->hdr.spi_header.proglen_1 = (sbuf.st_size >> 8) & 0xff;
        cb->hdr.spi_header.proglen_0 = (sbuf.st_size >> 0) & 0xff;
    }
    return 0;
}

static inline void reset_crc32(void)
{
    crc32 = 0xffffffff;
}

static inline unsigned long read_crc32(void)
{
    return crc32;
}

int do_crc32_program_data(struct cb *cb)
{
    FILE *fp;
    unsigned char buf[256];
    size_t bytes;
    int i;
    int bytes_read;
    
    fp = fopen(cb->binary_file_name, "r");
    if (!fp) {
        printf("Unable to open file %s\n", cb->binary_file_name);
        return -1;
    }
    
    reset_crc32();
    
    bytes_read = 0;
    while ((bytes = fread((void *)buf, 1, 256, fp)) > 0) {
        for (i = 0; i < bytes; i++) {
            update_crc32(buf[i]);
        }
        bytes_read += bytes;
    }
    
    /* If the file length is not 4 byte aligned then it will be padded with
    *  zeros when it is copied, which will affect the CRC.  Account for this
    *  here.
    */
    while ((bytes_read & 3) != 0) {
        update_crc32(0);
        bytes_read++;
    }
    
    fclose(fp);
    
    if (cb->nand) {
        cb->hdr.nand_header.bootstrap_crc_3 = (read_crc32() >> 24) & 0xff; 
        cb->hdr.nand_header.bootstrap_crc_2 = (read_crc32() >> 16) & 0xff; 
        cb->hdr.nand_header.bootstrap_crc_1 = (read_crc32() >> 8) & 0xff; 
        cb->hdr.nand_header.bootstrap_crc_0 = (read_crc32() >> 0) & 0xff; 
    } else {
        cb->hdr.spi_header.progcrc_3 = (read_crc32() >> 24) & 0xff;
        cb->hdr.spi_header.progcrc_2 = (read_crc32() >> 16) & 0xff;
        cb->hdr.spi_header.progcrc_1 = (read_crc32() >> 8) & 0xff;
        cb->hdr.spi_header.progcrc_0 = (read_crc32() >> 0) & 0xff;
    }
    
    return 0;
}

int do_crc32_hdr(struct cb *cb)
{
    reset_crc32();
    
    if (cb->nand) {
        update_crc32((unsigned char)(cb->hdr.nand_header.bootstrap_len_3));
        update_crc32((unsigned char)(cb->hdr.nand_header.bootstrap_len_2));
        update_crc32((unsigned char)(cb->hdr.nand_header.bootstrap_len_1));
        update_crc32((unsigned char)(cb->hdr.nand_header.bootstrap_len_0));
        update_crc32((unsigned char)(cb->hdr.nand_header.timing0_3));
        update_crc32((unsigned char)(cb->hdr.nand_header.timing0_2));
        update_crc32((unsigned char)(cb->hdr.nand_header.timing0_1));
        update_crc32((unsigned char)(cb->hdr.nand_header.timing0_0));
        update_crc32((unsigned char)(cb->hdr.nand_header.timing1_3));
        update_crc32((unsigned char)(cb->hdr.nand_header.timing1_2));
        update_crc32((unsigned char)(cb->hdr.nand_header.timing1_1));
        update_crc32((unsigned char)(cb->hdr.nand_header.timing1_0));
        update_crc32((unsigned char)(cb->hdr.nand_header.bootstrap_crc_3));
        update_crc32((unsigned char)(cb->hdr.nand_header.bootstrap_crc_2));
        update_crc32((unsigned char)(cb->hdr.nand_header.bootstrap_crc_1));
        update_crc32((unsigned char)(cb->hdr.nand_header.bootstrap_crc_0));
        
        cb->hdr.nand_header.header_crc_3 = ((read_crc32() >> 24) & 0xff);
        cb->hdr.nand_header.header_crc_2 = ((read_crc32() >> 16) & 0xff);
        cb->hdr.nand_header.header_crc_1 = ((read_crc32() >> 8) & 0xff);
        cb->hdr.nand_header.header_crc_0 = ((read_crc32() >> 0) & 0xff);
    } else {
        update_crc32((unsigned char)(cb->hdr.spi_header.spi_boot_sig1));
        update_crc32((unsigned char)(cb->hdr.spi_header.proglen_2));
        update_crc32((unsigned char)(cb->hdr.spi_header.proglen_1));
        update_crc32((unsigned char)(cb->hdr.spi_header.proglen_0));
        
        update_crc32((unsigned char)(cb->hdr.spi_header.progld_3));
        update_crc32((unsigned char)(cb->hdr.spi_header.progld_2));
        update_crc32((unsigned char)(cb->hdr.spi_header.progld_1));
        update_crc32((unsigned char)(cb->hdr.spi_header.progld_0));
        
        update_crc32((unsigned char)(cb->hdr.spi_header.progcrc_3));
        update_crc32((unsigned char)(cb->hdr.spi_header.progcrc_2));
        update_crc32((unsigned char)(cb->hdr.spi_header.progcrc_1));
        update_crc32((unsigned char)(cb->hdr.spi_header.progcrc_0));
        
        cb->hdr.spi_header.hdrcrc_3 = ((read_crc32() >> 24) & 0xff);
        cb->hdr.spi_header.hdrcrc_2 = ((read_crc32() >> 16) & 0xff);
        cb->hdr.spi_header.hdrcrc_1 = ((read_crc32() >> 8) & 0xff);
        cb->hdr.spi_header.hdrcrc_0 = ((read_crc32() >> 0) & 0xff);
    }
    
    return 0;
}

int do_csum(struct cb *cb)
{
    int rc = 0;
    
    rc = do_crc32_program_data(cb);
    if (rc == 0)
        rc = do_crc32_hdr(cb);
    return rc;
}

int write_output_file(struct cb *cb)
{
    FILE *fpd, *fps;
    unsigned char buf[256];
    unsigned char *hdr;
    int sizeof_header;
    int bytes_written;
    size_t bytes;
    
    fpd = fopen(cb->output_file_name, "w+");
    if (!fpd) {
        printf("Unable to create %s\n", cb->output_file_name);
        return -1;
    }
    
    fps = fopen(cb->binary_file_name, "r");
    if (!fps) {
        fclose(fpd);
        printf("Unable to open %s\n", cb->binary_file_name);
        return -1;
    }
    
    if (cb->nand) {
        hdr = (char *)&cb->hdr.nand_header;
        sizeof_header = sizeof(struct nand_header);
    } else {
        hdr = (char *)&cb->hdr.spi_header;
        sizeof_header = sizeof(struct spi_header);
    }
    /* Write header. */
    if (fwrite((void *)hdr, 1, sizeof_header, fpd) != sizeof_header) {
        fclose(fpd);
        fclose(fps);
        return -1;
    }
    
    /* Append binary data */
    bytes_written = 0;
    while ((bytes = fread((void *)buf, 1, 256, fps)) > 0) {
        if (fwrite((void *)buf, 1, bytes, fpd) != bytes) {
            fclose(fpd);
            fclose(fps);
            return -1;
        }
        bytes_written += bytes;
    }
    
    buf[0] = 0;
    while ((bytes_written & 3) != 0) {
        /* Pad out file to a 4 byte alignment with zeros */
        if (fwrite((void *)buf, 1, 1, fpd) != 1) {
            fclose(fpd);
            fclose(fps);
            return -1;
        }
        bytes_written++;
    }
    
close_out:
    fclose(fpd);
    fclose(fps);
    
    return 0;
}

void print_header(struct cb *cb)
{
    struct cb cb2;
    FILE *fp;
    int bytes;
    unsigned long len;

    /* Print header from output file. */    
    fp = fopen(cb->output_file_name, "r");
    if (!fp) {
        printf("Couldn't open %s to print out header\n", cb->output_file_name);
        return;
    }
    /* Read the largest header into cb2 union. */
    bytes = fread((void *)&cb2.hdr.nand_header, 1, sizeof(struct nand_header), fp);
    fclose(fp);
    if (bytes != sizeof(struct nand_header)) {
        printf("%s: unable to read header of size %d from %s\n", __FUNCTION__, sizeof(struct nand_header), cb->output_file_name);
        return;
    }
    if (cb2.hdr.spi_header.spi_boot_sig1 == SPI_BOOT_SIG) {
        len = cb2.hdr.spi_header.proglen_0 + (cb2.hdr.spi_header.proglen_1 << 8)
            + (cb2.hdr.spi_header.proglen_2 << 16);
        printf("Image                 :SPI\n");
        printf("Magic                 :0x%02x\n", cb2.hdr.spi_header.spi_boot_sig1);
        printf("Program length        :%lu\n", len);
    } else {
        unsigned long *pval;
        
        pval = (unsigned long *)&cb2.hdr.nand_header.magic_3;
        if (ntohl(*pval) == NAND_MAGIC) {
            printf("Image   :NAND\n");
            printf("Magic   :0x%08x\n", ntohl(*pval));
            pval = (unsigned long *)&cb2.hdr.nand_header.timing0_3;
            printf("Timing0 :0x%08x\n", ntohl(*pval));
            pval = (unsigned long *)&cb2.hdr.nand_header.timing1_3;
            printf("Timing1 :0x%08x\n", ntohl(*pval));
            pval = (unsigned long *)&cb2.hdr.nand_header.bootstrap_len_3;
            printf("Length  :%d\n", ntohl(*pval) << 2);
        } else {
            unsigned char *p = (unsigned char *)&cb2.hdr.nand_header;
            int i;
            printf("Error: header in %s not recognised:\n", cb->output_file_name);
            for (i = 0; i < sizeof(struct nand_header); i++) {
                printf("%02x ", *(p + i));
                if ((i + 1) % 8 == 0)
                    printf("\n");
            }
            if (i % 8)
                printf("\n");
        }
    }
}

void set_prog_name(char *argv0)
{
    char *p;
    
    p = strrchr(argv0, '/');
    if (p)
        p++;
    else
        p = argv0;
    prog_name = strdup(p);
}

int main(int argc, char **argv)
{
    int rc;
    struct cb cb;
    
    set_prog_name(argv[0]);
    /* Load command line parameters into cb structure. */
    rc = parse_cmd_line(&cb, argc, argv);
    if (rc != 0) {
        print_usage();
        return -1;
    }
    
    init_crc_table();
    
    /* Load the binary file length into cb structure. */
    rc = get_binary_file_length(&cb);
    if (rc != 0) {
        printf("File %s does not exist\n", cb.binary_file_name);
        return -1;
    }
    /* Apply the relevent checksum or crc. */
    rc = do_csum(&cb);
    if (rc != 0) {
        printf("Csum failed\n");
        return -1;  
    }
    /* Write the header and then append the binary file to the output file. */
    rc = write_output_file(&cb);
    if (rc != 0) {
        printf("Unable to write output file\n");
        return -1;
    }
    /* Read the header in from the binary file and print to output. */
    print_header(&cb);
    return 0;
}
