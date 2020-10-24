void read_sectors(enum fs fs, int sector, char **data, int sectors,
		  int drive, int fd, int cylinder, struct xdf_struct *fmt, 
		  int direction, struct floppy_raw_cmd *raw_cmd, int *j)
{
	int limit, head, psector, lsectors;

	while(sectors) {
		lsectors = sectors;
		head = 0;
		if(fs == MAIN_FS) {
			limit = fmt->sect_per_track_0 - 8;
			if(sector < limit) {
				psector = sector + 129;
				if(sector + lsectors > limit)
					lsectors = limit - sector;
			} else {
				head = 1;
				psector = sector - limit + 129;
			}
		} else {
			if(sector >= 8)
				return;
			psector = sector + 1;
			if(lsectors + sector >= 8)
				lsectors = 8 - sector;
		}
		readwrite_sectors(fd, drive, direction,
				  cylinder, head, psector, 
				  2, *data, lsectors << 9,
				  raw_cmd + (*j)++, fmt->rate);
		*data += lsectors << 9;
		sector += lsectors;
		sectors -= lsectors;
	}
}
