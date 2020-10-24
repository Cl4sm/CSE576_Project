                int interval,int drivetype) 
{
    int e,fd,disk_in,prev_disk_in,first;
    struct floppy_drive_struct state;
    mnt_node *mnt;

    strncpy(curdev,devname, sizeof(curdev));
    curdev[sizeof(curdev)-1]='\0';

    fd=open(devname,O_RDONLY|O_NDELAY);
    if (fd<0) {
	errmsg("error opening device: %s",strerror(errno));
	return -1;
    }

    prev_disk_in=0;
    first=1;

    ioctl(fd,FDFLUSH);
/*	close(fd);*/
    while(1) {
/*		fd=open(devname,O_RDONLY|O_NDELAY);
		if (fd<0) {
			errmsg("error opening device: %s",strerror(errno));
			return -1;
		}
		*/	
		usleep(interval*100000);
		e=ioctl(fd,FDPOLLDRVSTAT,&state);
		if (e) {
			msg("ioctl(FDPOLLDRVSTAT) failed: %s",strerror(errno));
			return -1;
		}
		printf("flags=%02lx\n", state.flags);
		disk_in=!(state.flags & (FD_VERIFY | FD_DISK_NEWCHANGE));
		if (disk_in && !prev_disk_in && !first) {
			msg("disk inserted");
			ioctl(fd,FDFLUSH);
			do_mount(devname,mountpoint,mountflags,0,1,drivetype);
		}
		if (!disk_in && prev_disk_in) {
			msg("disk removed");
			read_mtab();
			mnt=get_mounted(devname);
			if (mnt) {
				if (!hasmntopt(&mnt->ms,"sync") && !hasmntopt(&mnt->ms,"ro"))
					msg("arg!! wasn't mounted sync");
				/* should check for dirty buffers here! */
				do_umount(devname,0);
			}
			
		}
		prev_disk_in=disk_in;
		first=0;
/*		close(fd);*/
    }
}
