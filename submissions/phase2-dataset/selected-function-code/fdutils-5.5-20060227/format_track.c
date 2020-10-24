{
	format_map_t *data;
	struct floppy_raw_cmd raw_cmd;
	int offset;
	int i;
	int nssect;      
	int skew;
	
	data = (format_map_t *) floppy_buffer;

	/* place "fill" sectors */
	for (i=0; i<fd->nssect*2+1; ++i){
		data[i].sector = 128+i;
		data[i].size = /*fd->sizecode*/7;
		data[i].cylinder = cylinder;
		data[i].head = head;
	}

	if(do_skew) {
		fd += findex[cylinder][head];
		skew = fd->min + lskews[cylinder][head] * fd->chunksize;
		assert(skew >= fd->min);
		assert(skew <= fd->max);		
	} else
		skew = 0;

	/* place data sectors */
	nssect = 0;
	for (i=0; i<fd->dsect; ++i){
		offset = fd->sequence[i].offset + lskews[cylinder][head];
		offset = offset % fd->nssect;
		data[offset].sector = fd->sequence[i].sect - fd->zeroBased;
		data[offset].size = fd->sequence[i].size;
		data[offset].cylinder = cylinder;
		data[offset].head = head;
		if ( offset >= nssect )
			nssect = offset+1;
	}	
	if ( (nssect-1) * fd->chunksize > fd->raw_capacity - header_size - index_size){
		printf("Sector too far out %d*%d > %d-%d-%d!\n",
		       nssect , fd->chunksize, fd->raw_capacity , header_size,
		       index_size);
		exit(1);
	}

	/* debugging */
	if (verbosity == 9){
		printf("chunksize=%d\n", fd->chunksize);
		printf("sectors=%d\n", nssect);
		for (i=0; i<nssect; ++i)
			printf("%2d/%d, ", data[i].sector, data[i].size);
		printf("\n");
	}

	/* prepare command */
	raw_cmd.data = floppy_buffer;
	raw_cmd.length = nssect * sizeof(format_map_t);
	raw_cmd.cmd_count = 6;
	raw_cmd.cmd[0] = FD_FORMAT & ~fm_mode;
	raw_cmd.cmd[1] = (head << 2 | ( fd->drive & 3)) ^
	    (fd->swapSides ? 4 : 0);
	raw_cmd.cmd[2] = fd->sizecode;
	raw_cmd.cmd[3] = nssect;
	raw_cmd.cmd[4] = fd->fmt_gap;
	raw_cmd.cmd[5] = 0;
	raw_cmd.flags = FD_RAW_WRITE | FD_RAW_INTR | FD_RAW_SPIN | 
		FD_RAW_NEED_SEEK | FD_RAW_NEED_DISK;
	raw_cmd.track = cylinder << stretch;
	raw_cmd.rate = fd->rate & 0x43;

	/* first pass */
	if (verbosity >= 6)
		printf("formatting...\n");
	if(send_cmd(fd->fd, & raw_cmd, "format"))
		return -1;

	memset(floppy_buffer, 0, sizeof(floppy_buffer));
	if ( !fd->need_init && fd->sizecode)
		return 0;

	if (verbosity >= 6)
		printf("initializing...\n");
	return rw_track(fd, cylinder, head, 1);
}
