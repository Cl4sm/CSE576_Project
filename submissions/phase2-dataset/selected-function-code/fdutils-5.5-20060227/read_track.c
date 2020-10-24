{
	struct floppy_raw_cmd raw_cmd;
	int tmp;
	unsigned char ref;

	unsigned char buffer[32*1024];
	unsigned char c;
	int ptr;
	int i;
	
	raw_cmd.cmd_count = 9; 
	raw_cmd.cmd[0] = FD_READ & ~0x80; /* format command */
	raw_cmd.cmd[1] = dn /* drive */;
	raw_cmd.cmd[2] = 0; /* cylinder */
	raw_cmd.cmd[3] = 0; /* head */
	raw_cmd.cmd[4] = 1; /* sector */
	raw_cmd.cmd[5] = 8; /* sizecode */
	raw_cmd.cmd[6] = 1; /* sect per track */
	raw_cmd.cmd[7] = 0x1b; /* gap */
	raw_cmd.cmd[8] = 0xff; /* sizecode2 */
	raw_cmd.data = buffer;
	raw_cmd.length = 32 * 1024;
	raw_cmd.flags = FD_RAW_INTR | FD_RAW_NEED_SEEK | FD_RAW_NEED_DISK |
		FD_RAW_READ;
	raw_cmd.rate = rate;
	raw_cmd.track = cylinder;
	tmp = ioctl(fd, FDRAWCMD, & raw_cmd);
	if ( tmp < 0 ){
		perror("read");
		exit(1);
	}


	if((raw_cmd.reply[1] & ~0x20) |
	   (raw_cmd.reply[2] & ~0x20) |
	   raw_cmd.reply[3]) {
		int i;
		fprintf(stderr, "\nFatal error while measuring raw capacity\n");
		for(i=0; i < raw_cmd.reply_count; i++) {
			fprintf(stderr, "%d: %02x\n", i, raw_cmd.reply[i]);
		}
		exit(1);
	}

	ptr = 514;
	/* we look first for the place where the 0x4e's are going to stop */
	ref = buffer[ptr];
	while(buffer[ptr] == ref)
		ptr += 256;
	ptr -= 240;
	while(buffer[ptr] == ref)
		ptr += 16;
	ptr -= 15;
	while(buffer[ptr] == ref)
		ptr ++;
	/* we have now found the first byte after wrap-around */

	/* jump immediately to the supposed beginning */
	ptr += 210;
	c = buffer[ptr];
	while(buffer[ptr] == c)
		ptr--;


	for(i=0; i<16; i++) {
		if(byte_tab[i].value == c) {
			ptr -= byte_tab[i].offset;
			return ptr * 16 + i;
		}
	}
	return ptr * 16;
}
