int doPoll(int fd,
	   struct floppy_drive_params *dpr,
	   struct floppy_drive_struct *state)
{
	if(! (dpr->flags & FD_SILENT_DCL_CLEAR)) {
		/* Work around a bug in floppy driver when silent dcl is not
		   set */
		struct floppy_raw_cmd raw_cmd;
		int fd2;
		fd2=open(procFd, 3 | O_NDELAY);
		if(fd2 != -1)
			close(fd2);
		/* Perform "dummy" rawcmd to flush out newchange */
		raw_cmd.flags = FD_RAW_NEED_DISK;
		raw_cmd.cmd_count = 0;
		raw_cmd.track = 0;
		ioctl(fd, FDRAWCMD, &raw_cmd, "rawcmd");
	}
	eioctl(fd, FDPOLLDRVSTAT, state, "reset");
	return state->flags;
}
