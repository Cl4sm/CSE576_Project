static int rw_track(struct params *fd, int cylinder, int head, int mode)
{
	int i;
	int cur_sector;
	int retries;
	struct floppy_raw_cmd raw_cmd;

	cur_sector = 1 - fd->zeroBased;

	for (i=MAX_SIZECODE-1; i>=0; --i) {
		if ( fd->last_sect[i] <= cur_sector + fd->zeroBased)
			continue;
		retries=0;
	retry:
		/* second pass */
		raw_cmd.data = floppy_buffer;
		raw_cmd.cmd_count = 9;
		raw_cmd.cmd[0] =
			(mode ? FD_WRITE : FD_READ) & ~fm_mode & ~0x80;
		raw_cmd.cmd[1] = (head << 2 | ( fd->drive & 3)) ^
		    (fd->swapSides ? 4 : 0);
		raw_cmd.cmd[2] = cylinder;
		raw_cmd.cmd[3] = head;
		raw_cmd.cmd[4] = cur_sector;
		raw_cmd.cmd[5] = i;
		raw_cmd.cmd[6] = fd->last_sect[i] - 1 - fd->zeroBased;
		raw_cmd.cmd[7] = fd->gap;
		if ( i )
			raw_cmd.cmd[8] = 0xff;
		else
			raw_cmd.cmd[8] = 0xff;
		raw_cmd.flags = (mode ? FD_RAW_WRITE : FD_RAW_READ) | 
			FD_RAW_INTR | FD_RAW_SPIN |
			FD_RAW_NEED_SEEK | FD_RAW_NEED_DISK;
		raw_cmd.track = cylinder << stretch;
		raw_cmd.rate = fd->rate & 0x43;

		raw_cmd.length = (fd->last_sect[i] - 
				  fd->zeroBased - 
				  cur_sector) * 128 << i;
		/* debugging */
		if (verbosity == 9)
			printf("%s %ld sectors of size %d starting at %d\n",
			       mode ? "writing" : "reading",
			       raw_cmd.length / 512, i, cur_sector);
		if(send_cmd(fd->fd, & raw_cmd, 
			    mode ? "format" : "verify")){
			if ( !retries && mode && (raw_cmd.reply[1] & ST1_ND) ){
				cur_sector = raw_cmd.reply[5];
				retries++;
				goto retry;
			}
			return -1;
		}
		cur_sector = fd->last_sect[i];
	}
	return 0;
}
