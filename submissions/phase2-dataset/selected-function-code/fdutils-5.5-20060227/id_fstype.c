
#define minix ((struct minix_super_block*)(super+1024))
#define dos   ((struct msdos_boot_sector*)super)
#define ext2  ((struct ext2_super_block*)(super+1024))
#define ext   ((struct ext_super_block*)(super+1024))
#define xia   ((struct xiafs_super_block*)(super+1024))

    memset(fmt,0,sizeof(*fmt));

    /* we look for Unix-type filesystems first because mkfs doesn't
       overwrite the first 1K of the disk, so a DOS filesystem might
       be detected even though the data area is overwritten.
       */

    /* look for Minix filesystem */

    if (minix->s_magic==MINIX_SUPER_MAGIC ||
	minix->s_magic==MINIX_SUPER_MAGIC2)
	{
	    fmt->totsect=minix->s_nzones * (2<<minix->s_log_zone_size);
	    return T_MINIX;
	}

    /* look for ext2 filesystem */

    if (ext2->s_magic==EXT2_SUPER_MAGIC
#ifdef EXT2_PRE_02B_MAGIC
	|| ext2->s_magic==EXT2_PRE_02B_MAGIC
#endif
	)
	{
	    fmt->totsect=ext2->s_blocks_count * (2<<ext2->s_log_block_size);
	    return T_EXT2;
	}

#ifdef EXT_SUPER_MAGIC
    /* look for ext filesystem */

    if (ext->s_magic==EXT_SUPER_MAGIC)
	{
	    fmt->totsect=ext->s_nzones * (2<<ext->s_log_zone_size);
	    return T_EXT;
	}
#endif

#ifdef _XIAFS_SUPER_MAGIC
    /* look for xia filesystem */

    if (xia->s_magic==_XIAFS_SUPER_MAGIC)
	{
	    fmt->totsect=xia->s_nzones * xia->s_zone_size/512;
	    return T_XIA;
	}
#endif
    /* add more format types here ... */

    /* look for MS-DOG filesystem 
       this is more a looking for hints than checking a 
       well-defined identification (which doesn't exist), and 
       I don't expect it to be 100% reliable. 
       */
   
    if (dos->bootid==0xAA55 ||             /* check boot sector id */
	(*(byte*)(super+512)>=0xF0 &&      /* check media descriptor */
	 *(word*)(super+513)==0xFFFF) ||
        strncmp(dos->fat_type,"FAT",3)==0) /* check FAT id string */ 
	{
	    int sect,heads,tracks,totsect,s_cyl;

	    totsect=dos->sectors;
	    if (totsect==0) totsect=dos->total_sect;
	    sect=dos->secs_track;
	    heads=dos->heads;
	    s_cyl=sect*heads;

	    /* sanity checks */
	    if (!INRANGE(heads,1,2) || 
		!INRANGE(sect,3,60) ||
		!INRANGE(totsect,100,10000) ||
		totsect % s_cyl != 0)
		{
		    /* try media descriptor (very old DOS disks) */
		    switch(super[512]) {
			case 0xfe:             /* 160k */
			    tracks=40; 
			    sect=8; 
			    heads=1;
			    break;
			case 0xfc:             /* 180k */
			    tracks=40;
			    sect=9;
			    heads=1;
			    break;
			case 0xff:             /* 320k */
			    tracks=40;
			    sect=8;
			    heads=2;
			    break;
			case 0xfd:             /* 360k */
			    tracks=40;
			    sect=9;
			    heads=2;
			    break;
			case 0xf9:             /* 1.2M */
			    tracks=80;
			    sect=15;
			    heads=2;
			    break;
			default:
			    goto no_dos;
		    }
		    totsect=tracks*sect*heads;
		}
	    else {
		tracks=totsect/s_cyl;
	    }

	    fmt->sect    = sect;
	    fmt->tracks  = tracks;
	    fmt->heads   = heads;
	    fmt->totsect = totsect;
	    fmt->_2m=0;
	    fmt->sectsizec=2;  /* 512 bytes */
      
	    /* check for 2M format */
	    if (strncmp(dos->banner,"2M",2)==0) {
		int inftm=dos->InfTm;
		if (INRANGE(inftm,76,510)) {
		    fmt->sectsizec=super[inftm];
		    if (!INRANGE(fmt->sectsizec,0,7)) fmt->sectsizec=2;
		}
		fmt->_2m=1;
	    }
	    else {
		fmt->sectsizec=2;
	    }
	    return T_DOS;
	}

no_dos:

    return 0;  /* disk format not identified. */
   
}
