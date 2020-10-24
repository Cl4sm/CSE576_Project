static void m_format_track(int fd, int dn, int rate, int cylinder)
{
	struct floppy_raw_cmd raw_cmd;
	int tmp;
	format_map_t format_map[1] = {
		{ 0, 0, 1, 8 }
	};
	
	raw_cmd.cmd_count = 6; 
	raw_cmd.cmd[0] = FD_FORMAT; /* format command */
	raw_cmd.cmd[1] = dn /* drive */;
	raw_cmd.cmd[2] = 2; /* sizecode used for formatting */
	raw_cmd.cmd[3] = 1; /* sectors per track */
	raw_cmd.cmd[4] = 255; /* gap. unimportant anyways */
	raw_cmd.cmd[5] = 3;
	raw_cmd.data = format_map;
	raw_cmd.length = sizeof(format_map);
	raw_cmd.flags = FD_RAW_INTR | FD_RAW_NEED_SEEK | FD_RAW_NEED_DISK |
		FD_RAW_WRITE;
	raw_cmd.rate = rate;
	raw_cmd.track = cylinder;

	tmp = ioctl(fd, FDRAWCMD, & raw_cmd);
	if ( tmp < 0 ){
		perror("format");
		exit(1);
	}

	if((raw_cmd.reply[1] & ~0x20) |
	   (raw_cmd.reply[2] & ~0x20)) {
		int i;

		if ( raw_cmd.reply[1] & ST1_WP ){
		    fprintf(stderr,"The disk is write protected\n");
		    exit(1);
		}

		fprintf(stderr, 
			"\nFatal error while measuring raw capacity\n");
		for(i=0; i < raw_cmd.reply_count; i++) {
			fprintf(stderr, "%d: %02x\n", i, raw_cmd.reply[i]);
		}
		exit(1);
	}
}
