#ifndef _ROMFS_H_
#define _ROMFS_H_

#include <stdio.h>
#include <string.h>

#define ROMFH_HRD   0 /* 硬链接文件 */
#define ROMFH_DIR   1 /* 目录文件 */
#define ROMFH_REG   2 /* 普通文件 */
#define ROMFH_LNK   3 /* 链接文件 */
#define ROMFH_BLK   4 /* 块设备文件 */
#define ROMFH_CHR   5 /* 字符设备文件 */
#define ROMFH_SCK   6 /* 套接字文件 */
#define ROMFH_FIF   7
#define ROMFH_EXEC  8

#define ntohl(x)     (x)

#define ROMSB_WORD0 (0x2D726F6D)//('-','r','o','m')
#define ROMSB_WORD1 (0x3166732D)//('1','f','s','-')

#define ROMFS_NAME_SIZE     (16)

 
#define ROMFS_READWIND_SIZE     (32)    /* 搜索时读取的字节数*/      

#define ROMFS_BUFLEN            (ROMFS_READWIND_SIZE)


/*
offset      content
        +---+---+---+---+
  0     |  -  |  r  |  o  | m  |          +---+---+---+---+    The ASCII representation of those bytes
  4     |  1  |  f  |  s  |  -  |  /    (i.e. "-rom1fs-")
        +---+---+---+---+
  8     |     full size       |    The number of accessible bytes in this fs.
        +---+---+---+---+
 12     |    checksum   |       The checksum of the FIRST 512 BYTES.
        +---+---+---+---+
 16     |  volume name     |    The zero terminated name of the volume,
        :               :       padded to 16 byte boundary.
        +---+---+---+---+
 xx     |     file      |
        :    headers    :
*/

typedef unsigned char uint8;

typedef unsigned int uint32;

typedef enum _ROMFS_RET_
{
    eromfs_ok,
    eromfs_var_err,
    eromfs_data_err,
    eromfs_no_thisfil,
}eromfs;


/* On-disk "super block" */
typedef struct _st_romfs_super_block {
        uint8 word0[4];
        uint8 word1[4];
        uint8 size[4];
        uint8 checksum[4];
        char name[0];           /* volume name */
}romfs_superblock;

/*
offset      content
        +---+---+---+---+
        |     file header    ｜      
        +---+---+---+---+      
        |      file date     ｜      
        +---+---+---+---+      
        |     file header    ｜      
        +---+---+---+---+      
        |      file date     ｜      
        |     file header    ｜      
        +---+---+---+---+      
        |      file date     ｜      
        +---+---+---+---+      
        |      …….        ｜      
        +---+---+---+---+   
offset      content
        +---+---+---+---+
  0     | next filehdr   | X  |       The offset of the next file header
        +---+---+---+---+         (zero if no more files)
  4     |   spec.info       |       Info for directories/hard links/devices
        +---+---+---+---+
  8     |        size       |       The size of this file in bytes
        +---+---+---+---+
 12     |    checksum     |       Covering the meta data, including the file
        +---+---+---+---+         name, and padding
 16     |    file name      |       The zero terminated name of the file,
        :                    :       padded to 16 byte boundary*/
/* On disk inode */
typedef struct _st_romfs_inode{
        uint8 next[4];            /* low 4 bits see ROMFH_ */
        uint8 spec[4];
        uint8 size[4];
        uint8 checksum[4];            /* check sum of file header and file name */
        char name[0];
}romfs_fileheader;

typedef struct _ST_ROMFS_FIL_
{
    uint32 fileheader;
    //uint32 filept;                  /* 文件正文偏移地址 */
    uint32 filesize;                /* 文件长度 */
    uint32 fseek;                   /* 文件读写指针*/
    char name[ROMFS_NAME_SIZE];
 }romfsFIL;

typedef struct _ST_ROMFS_FAT_
{
    romfs_superblock romfsspblk;    /* 文件系统超级块 */
    romfs_fileheader fileheader;    /* 第一个文件头 */
    romfsFIL startfile;             /* 第一个文件描述数据 */
    uint32 romfssize;               /* 文件系统长度 */
    //uint32 fstfileheader;           /* 第一个文件的文件头位置 */
    //uint32 endfileheader;           /* 最后个文件的文件头位置 */
    uint32 readpt;                  /* 读写位置指针 */
    uint32 readwind_size;           /* 读写长度计数 */
    uint8  romfsbuf[ROMFS_BUFLEN];  /* 读写缓冲区 */
}stRomfsFAT;


/**************************************************************
* 函数名：	 
* 函数介绍: 
* 输入参数: 
* 输出参数: 
* 返回值:	 
* 作者:	 by longj 2017-11-09
**************************************************************/

extern eromfs romfs_mount(void);
/**************************************************************
* 函数名：	 
* 函数介绍: 
* 输入参数: 
* 输出参数: 
* 返回值:	 
* 作者:	 by longj 2017-11-09
**************************************************************/

extern eromfs romfs_fopen(romfsFIL *fp, const char *filename ,uint32 mode);
/**************************************************************
* 函数名：	 
* 函数介绍: 
* 输入参数: 
* 输出参数: 
* 返回值:	 
* 作者:	 by longj 2017-11-09
**************************************************************/

extern eromfs romfs_fread(
    romfsFIL *fp, 
	void* buff,		/* Pointer to data buffer */
	uint32 btr,		/* Number of bytes to read */
	uint32* br		/* Pointer to number of bytes read */
);

#endif



