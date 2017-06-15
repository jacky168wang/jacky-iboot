#ifndef _LIB_FS_UBI_H_
#define _LIB_FS_UBI_H_

/* Fusiv Specific */
#define ECC_ENABLE  1
//#define DEBUG_FBOOT   1

#define MAX_VOLUMES_UBOOT   2

#define NAND_UBIPART_BASEADDR   0x20000
#define DDR_UBOOT_TEXTADDR      0xA0800000
#define NAND_PAGE_SIZE          0x800
#define NAND_BLOCK_SIZE         0x20000
#define NAND_FLASH_SIZE         0x20000000 //512MB
#define VOLUMEID_OFFSET     0x8
#define LAYOUT_VOLUMEID     0x7fffefff

#define DDR_BUFFER_ADDR         0x80400000 

#define DDR_BUFFER2_ADDR        	0x80100000 

#define NAND_ECC_BYTE4			4
#define NAND_ECC_BYTE5			5
#define NAND_MAX_COUNT_ZERO		12
#define NAND_MIN_COUNT_ZERO		8

#define UBOOT_TEXT_BASE 		0xA0800000

#define NAND_PAGE_SIZE_4KB		0x1000
#define NAND_BLOCK_SIZE_512KB		0x80000
#define NAND_CHIP_SIZE_1GB      	0x40000000
#define NAND_PAGE_SHIFT_4KB		13
#define NAND_UBIPART_BASEADDR_512KB     0x80000
#define NAND_OOB_224			224
#define NAND_MAX_OFFSET_1GB             0x3FF40000

#define NAND_PAGE_SIZE_2KB		0x800
#define NAND_BLOCK_SIZE_128KB           0x20000
#define NAND_CHIP_SIZE_512MB            0x20000000
#define NAND_PAGE_SHIFT_2KB		12
#define NAND_UBIPART_BASEADDR_128KB     0x20000
#define NAND_OOB_64			64
#define NAND_MAX_OFFSET_512MB           0x1FF40000

/* UBI Specific definitions */

/* The maximum number of volumes per one UBI device */
#define UBI_MAX_VOLUMES 128

/* The maximum volume name length */
#define UBI_VOL_NAME_MAX 127

/* The initial CRC32 value used when calculating CRC checksums */
#define UBI_CRC32_INIT 0xFFFFFFFFU
                
/* Erase counter header magic number (ASCII "UBI#") */
#define UBI_EC_HDR_MAGIC  0x55424923
/* Volume identifier header magic number (ASCII "UBI!") */
#define UBI_VID_HDR_MAGIC 0x55424921

/* Sizes of UBI headers */
#define UBI_EC_HDR_SIZE  sizeof(struct ubi_ec_hdr)
#define UBI_VID_HDR_SIZE sizeof(struct ubi_vid_hdr) 

/* Sizes of UBI headers without the ending CRC */
#define UBI_EC_HDR_SIZE_CRC  (UBI_EC_HDR_SIZE  - sizeof(__be32))
#define UBI_VID_HDR_SIZE_CRC (UBI_VID_HDR_SIZE - sizeof(__be32))

/* Size of the volume table record */
#define UBI_VTBL_RECORD_SIZE sizeof(struct ubi_vtbl_record)
/* Size of the volume table record without the ending CRC */
#define UBI_VTBL_RECORD_SIZE_CRC (UBI_VTBL_RECORD_SIZE - sizeof(__be32))


/**
 * struct ubi_vid_hdr - on-flash UBI volume identifier header.
 * @magic: volume identifier header magic number (%UBI_VID_HDR_MAGIC)
 * @version: UBI implementation version which is supposed to accept this UBI
 * image (%UBI_VERSION)
 * @vol_type: volume type (%UBI_VID_DYNAMIC or %UBI_VID_STATIC)
 * @copy_flag: if this logical eraseblock was copied from another physical
 * eraseblock (for wear-leveling reasons)
 * @compat: compatibility of this volume (%0, %UBI_COMPAT_DELETE,
 * %UBI_COMPAT_IGNORE, %UBI_COMPAT_PRESERVE, or %UBI_COMPAT_REJECT)
 * @vol_id: ID of this volume
 * @lnum: logical eraseblock number
 * @leb_ver: version of this logical eraseblock (IMPORTANT: obsolete, to be
 * removed, kept only for not breaking older UBI users)
 * @data_size: how many bytes of data this logical eraseblock contains
 * @used_ebs: total number of used logical eraseblocks in this volume
 * @data_pad: how many bytes at the end of this physical eraseblock are not
 * used
 * @data_crc: CRC checksum of the data stored in this logical eraseblock
 * @padding1: reserved for future, zeroes
 * @sqnum: sequence number
 * @padding2: reserved for future, zeroes
 * @hdr_crc: volume identifier header CRC checksum
 *
 * The @sqnum is the value of the global sequence counter at the time when this
 * VID header was created. The global sequence counter is incremented each time
 * UBI writes a new VID header to the flash, i.e. when it maps a logical
 * eraseblock to a new physical eraseblock. The global sequence counter is an
 * unsigned 64-bit integer and we assume it never overflows. The @sqnum
 * (sequence number) is used to distinguish between older and newer versions of
 * logical eraseblocks.
 *
 * There are 2 situations when there may be more then one physical eraseblock
 * corresponding to the same logical eraseblock, i.e., having the same @vol_id
 * and @lnum values in the volume identifier header. Suppose we have a logical
 * eraseblock L and it is mapped to the physical eraseblock P.
 *
 * 1. Because UBI may erase physical eraseblocks asynchronously, the following
 * situation is possible: L is asynchronously erased, so P is scheduled for
 * erasure, then L is written to,i.e. mapped to another physical eraseblock P1,
 * so P1 is written to, then an unclean reboot happens. Result - there are 2
 * physical eraseblocks P and P1 corresponding to the same logical eraseblock
 * L. But P1 has greater sequence number, so UBI picks P1 when it attaches the
 * flash.
 *
 * 2. From time to time UBI moves logical eraseblocks to other physical
 * eraseblocks for wear-leveling reasons. If, for example, UBI moves L from P
 * to P1, and an unclean reboot happens before P is physically erased, there
 * are two physical eraseblocks P and P1 corresponding to L and UBI has to
 * select one of them when the flash is attached. The @sqnum field says which
 * PEB is the original (obviously P will have lower @sqnum) and the copy. But
 * it is not enough to select the physical eraseblock with the higher sequence
 * number, because the unclean reboot could have happen in the middle of the
 * copying process, so the data in P is corrupted. It is also not enough to
 * just select the physical eraseblock with lower sequence number, because the
 * data there may be old (consider a case if more data was added to P1 after
 * the copying). Moreover, the unclean reboot may happen when the erasure of P
 * was just started, so it result in unstable P, which is "mostly" OK, but
 * still has unstable bits.
 *
 * UBI uses the @copy_flag field to indicate that this logical eraseblock is a
 * copy. UBI also calculates data CRC when the data is moved and stores it at
 * the @data_crc field of the copy (P1). So when UBI needs to pick one physical
 * eraseblock of two (P or P1), the @copy_flag of the newer one (P1) is
 * examined. If it is cleared, the situation* is simple and the newer one is
 * picked. If it is set, the data CRC of the copy (P1) is examined. If the CRC
 * checksum is correct, this physical eraseblock is selected (P1). Otherwise
 * the older one (P) is selected.
 *
 * Note, there is an obsolete @leb_ver field which was used instead of @sqnum
 * in the past. But it is not used anymore and we keep it in order to be able
 * to deal with old UBI images. It will be removed at some point.
 *
 * There are 2 sorts of volumes in UBI: user volumes and internal volumes.
 * Internal volumes are not seen from outside and are used for various internal
 * UBI purposes. In this implementation there is only one internal volume - the
 * layout volume. Internal volumes are the main mechanism of UBI extensions.
 * For example, in future one may introduce a journal internal volume. Internal
 * volumes have their own reserved range of IDs.
 *
 * The @compat field is only used for internal volumes and contains the "degree
 * of their compatibility". It is always zero for user volumes. This field
 * provides a mechanism to introduce UBI extensions and to be still compatible
 * with older UBI binaries. For example, if someone introduced a journal in
 * future, he would probably use %UBI_COMPAT_DELETE compatibility for the
 * journal volume.  And in this case, older UBI binaries, which know nothing
 * about the journal volume, would just delete this volume and work perfectly
 * fine. This is similar to what Ext2fs does when it is fed by an Ext3fs image
 * - it just ignores the Ext3fs journal.
 *
 * The @data_crc field contains the CRC checksum of the contents of the logical
 * eraseblock if this is a static volume. In case of dynamic volumes, it does
 * not contain the CRC checksum as a rule. The only exception is when the
 * data of the physical eraseblock was moved by the wear-leveling unit, then
 * the wear-leveling unit calculates the data CRC and stores it in the
 * @data_crc field. And of course, the @copy_flag is %in this case.
 *
 * The @data_size field is used only for static volumes because UBI has to know
 * how many bytes of data are stored in this eraseblock. For dynamic volumes,
 * this field usually contains zero. The only exception is when the data of the
 * physical eraseblock was moved to another physical eraseblock for
 * wear-leveling reasons. In this case, UBI calculates CRC checksum of the
 * contents and uses both @data_crc and @data_size fields. In this case, the
 * @data_size field contains data size.
 *
 * The @used_ebs field is used only for static volumes and indicates how many
 * eraseblocks the data of the volume takes. For dynamic volumes this field is
 * not used and always contains zero.
 *
 * The @data_pad is calculated when volumes are created using the alignment
 * parameter. So, effectively, the @data_pad field reduces the size of logical
 * eraseblocks of this volume. This is very handy when one uses block-oriented
 * software (say, cramfs) on top of the UBI volume.
 */
struct ubi_vid_hdr {
        __be32  magic;
        __u8    version;
        __u8    vol_type;
        __u8    copy_flag;
        __u8    compat;     
        __be32  vol_id;   
        __be32  lnum;
        __be32  leb_ver; /* obsolete, to be removed, don't use */
        __be32  data_size;
        __be32  used_ebs;
        __be32  data_pad;
        __be32  data_crc;
        __u8    padding1[4];
        __be64  sqnum;
        __u8    padding2[12];
        __be32  hdr_crc;
} __attribute__ ((packed));

/**
 * struct ubi_vtbl_record - a record in the volume table.
 * @reserved_pebs: how many physical eraseblocks are reserved for this volume
 * @alignment: volume alignment
 * @data_pad: how many bytes are unused at the end of the each physical
 * eraseblock to satisfy the requested alignment
 * @vol_type: volume type (%UBI_DYNAMIC_VOLUME or %UBI_STATIC_VOLUME)
 * @upd_marker: if volume update was started but not finished
 * @name_len: volume name length
 * @name: the volume name
 * @flags: volume flags (%UBI_VTBL_AUTORESIZE_FLG)
 * @padding: reserved, zeroes
 * @crc: a CRC32 checksum of the record
 *
 * The volume table records are stored in the volume table, which is stored in
 * the layout volume. The layout volume consists of 2 logical eraseblock, each
 * of which contains a copy of the volume table (i.e., the volume table is
 * duplicated). The volume table is an array of &struct ubi_vtbl_record
 * objects indexed by the volume ID.
 *          
 * If the size of the logical eraseblock is large enough to fit
 * %UBI_MAX_VOLUMES records, the volume table contains %UBI_MAX_VOLUMES
 * records. Otherwise, it contains as many records as it can fit (i.e., size of
 * logical eraseblock divided by sizeof(struct ubi_vtbl_record)).
 *
 * The @upd_marker flag is used to implement volume update. It is set to %1
 * before update and set to %0 after the update. So if the update operation was
 * interrupted, UBI knows that the volume is corrupted.
 *                  
 * The @alignment field is specified when the volume is created and cannot be
 * later changed. It may be useful, for example, when a block-oriented file
 * system works on top of UBI. The @data_pad field is calculated using the
 * logical eraseblock size and @alignment. The alignment must be multiple to the
 * minimal flash I/O unit. If @alignment is 1, all the available space of
 * the physical eraseblocks is used.
 *
 * Empty records contain all zeroes and the CRC checksum of those zeroes.
 */
struct ubi_vtbl_record {
        __be32  reserved_pebs;
        __be32  alignment;
        __be32  data_pad;
        __u8    vol_type;
        __u8    upd_marker;
        __be16  name_len;
        __u8    name[UBI_VOL_NAME_MAX+1];
        __u8    flags;
        __u8    padding[23];
        __be32  crc;
} __attribute__ ((packed));

extern unsigned int crc32_le(unsigned int crc, unsigned char const *p, size_t len);
/* extern unsigned int crc32_be(unsigned int crc, unsigned char const *p, size_t len); */

#define crc32(seed, data, length)  crc32_le(seed, (unsigned char const *)data, length)

#endif /* _LIB_FS_UBI_H_ */
