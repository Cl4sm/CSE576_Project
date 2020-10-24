			int cylinder, char *data, struct xdf_struct *fmt,
			int FatSize, int RootDirSize)
{
    struct floppy_raw_cmd raw_cmd[20];
    int i,j;
    struct timeval tv1, tv2;
    static struct timeval tv3, tv4;
    sector_map *map;

    j=i=0;
    if(cylinder) {
	    map = fmt->map;
	    while(map->size){
		    readwrite_sectors(fd, drive, direction,
				      cylinder, map->head, 
				      map->size + 128, 
				      map->size,
				      data, 128 << map->size, 
				      raw_cmd + j++, fmt->rate);
		    data += 128 << map->size;
		    map++;
		    i++;
	    }
    } else {
	    /* the boot sector & the FAT */
	    read_sectors(MAIN_FS, 0, &data, 1 + FatSize,
			 drive, fd, cylinder, fmt, direction, raw_cmd, &j);
	    
	    /* the index fs */
	    read_sectors(AUX_FS, 0, &data, 8,
			 drive, fd, cylinder, fmt, direction, raw_cmd, &j);

	    if( direction == READ_) {
		    /* the remaining sectors of the phantom FAT */
		    read_sectors(MAIN_FS, 9, &data, FatSize - 8,
				 drive, fd, cylinder, fmt, direction, raw_cmd, &j);
	    } else
		    data += (FatSize - 8) * 512;

	    read_sectors(MAIN_FS, 1 + FatSize, & data, RootDirSize,
			 drive, fd, cylinder, fmt, direction, raw_cmd, &j);

	    if (direction == READ_)
		    read_sectors(AUX_FS, 3, &data, 5,
				 drive,fd,cylinder,fmt, direction, raw_cmd, &j);
	    else
		    data += 5 * 512;
	    read_sectors(MAIN_FS,
			 1 + FatSize + RootDirSize + fmt->rootskip,
			 &data, 
			 fmt->sect_per_track_any * 2 - RootDirSize -
			 2 * FatSize - 6,
			 drive, fd, cylinder, fmt, direction, raw_cmd, &j);
    }

    raw_cmd[j-1].flags &= ~ FD_RAW_MORE;
    if(debug)
	    gettimeofday(&tv1,0);
    send_cmd(fd, raw_cmd, "read/write", 0);
    if(debug) {
	    gettimeofday(&tv2,0);
	    printf("\ncylinder %d: %ld %ld %ld\n\n", cylinder,
		   (long) ((tv2.tv_sec-tv1.tv_sec) * 1000000 + 
			   tv2.tv_usec - tv1.tv_usec),
		   (long) ((tv2.tv_sec-tv1.tv_sec) * 1000000 + 
			   tv2.tv_usec - tv1.tv_usec),
		   (long) ((tv2.tv_sec-tv4.tv_sec) * 1000000 + 
			   tv2.tv_usec - tv4.tv_usec));
	    if(cylinder == 2)
		    tv4 = tv2;
	    tv3 = tv2;
    }
}
