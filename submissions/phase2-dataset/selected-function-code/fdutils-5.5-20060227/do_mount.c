int do_mount(char *devname,char *_mountpoint,
             int flags,int force,int is_default,int drivetype) 
{
    int fd,e,fstype;
    format F;
    char *fsname;
    fmt_descr fmt;
    struct mntent ms;
    struct floppy_drive_struct drivstat;
    char options[80+MAX_OPT]; 
    char super[2048];
    char *mountpoint;
   
    strncpy(curdev,devname, sizeof(curdev));
    curdev[sizeof(curdev)-1]='\0';

    if (access(devname,R_OK)!=0)
	die("no access to %s",devname);
   
    if (!(flags&MS_RDONLY) && access(devname,W_OK)!=0)
	die("no write access to %s",devname);
      
    lock_read_mtab();
   
    if(get_mounted(devname)) {
	if (!force) {
	    errmsg("already mounted");
	    goto err_unlock;
	}
	else {
	    msg("already in /etc/mtab, trying to mount anyway!");
	}
    }

    mountpoint=canonicalize(_mountpoint);
   
    if (!chk_mountpoint(mountpoint,is_default)) goto err_unlock;

    /* all right, it's ok to mount. Now try to figure out
       the details.
       */

    fd=open(devname,O_RDONLY);
    if (fd<0) {
	errmsg("error opening device: %s",strerror(errno));
	goto err_unlock;
    }

    errno=0;   
    lseek(fd,0,SEEK_SET);
    read(fd,super,sizeof(super));
    if (errno) {
	errmsg("error reading boot/super block: %s",strerror(errno));
	goto err_close;
    }

    /* check if disk is write protected
       note: we don't need to poll (FDPOLLDRVSTAT) here because 
       the previous super block read updated the state.
       */
    DO_IOCTL(fd,FDGETDRVSTAT, &drivstat);
    if (!(drivstat.flags & FD_DISK_WRITABLE)) flags|=MS_RDONLY;

    /* get the auto-detected floppy parameters */
    DO_IOCTL(fd,FDGETPRM,&F);

#if DEBUG
    printf("autodetected format:\n\n");
    print_format(&F);
#endif
   
    fstype=id_fstype(super,&fmt);
    if (fstype==0) {
	errmsg("unknown filesystem type");
	goto err_close;
    }
    fsname=fsnames[fstype];

    if (fstype==T_DOS) {
	F.sect  = fmt.sect;
	F.track = fmt.tracks;
	F.head  = fmt.heads;
	F.size  = fmt.totsect;
	F.rate &= ~FD_2M;
	if (fmt._2m) {
	    F.rate &= ~0x38;
	    F.rate |= (((fmt.sectsizec+6)%8)<<3) | FD_2M;
	}
    }
    else {
	/* hope that the track layout was detected correctly and figure
	   out the number of tracks from the fs size.
	   */
	int s_cyl = F.sect*F.head;
	int tr;

	if (!s_cyl) goto err_close;

	tr=fmt.totsect/s_cyl;
	if (fmt.totsect%s_cyl==0 && INRANGE(tr,30,83)) {
	    /* was detected OK! */
	    F.track=tr;
	    F.size=fmt.totsect;
	}
	else {
	    errmsg("sorry, can't figure out format (%s filesystem)", fsname);
	    goto err_close;
	}
    }
    F.stretch = (drivetype!=360 && F.track<43);

#if DEBUG
    printf("setting format:\n\n");
    print_format(&F);
#endif
   
    DO_IOCTL(fd,FDSETPRM,&F);

    close(fd);  


    /* prepare the /etc/mtab entry and mount the floppy.
     */

    if (fstype==T_DOS) flags &= ~(MS_NOEXEC|MS_NODEV);
   
    *options=0;
    strcat(options,(flags&MS_RDONLY) ? "ro"      : "rw");
    strcat(options,(flags&MS_NOSUID) ? ",nosuid" : "");
    strcat(options,(flags&MS_NODEV)  ? ",nodev"  : "");
    strcat(options,(flags&MS_NOEXEC) ? ",noexec" : "");
    strcat(options,(flags&MS_SYNCHRONOUS) ? ",sync" : "");

    if(fstype == T_EXT2) {
	ADD_OPT("resuid=%d", ruid);
	/* resuid doesn't change the owner of the fs, but rather names the
	 * user who is allowed to fill up the fs more than 100%.
	 * This is just fine for use as a marker */
	strcat(options, ext2_options);
    }
    if(fstype == T_DOS)
	strcat(options, dos_options);

    if(fstype != T_EXT2) {
	/* Unfortunately, ext2 barfs at options it doesn't understand */
	ADD_OPT("uid=%d", ruid);
	ADD_OPT("gid=%d", rgid);
	ADD_OPT("umask=%03o", _umask);
    }

    e=mount(devname,mountpoint,fsname,flags|MS_MGC_VAL,NULL);
    if (e && fstype==T_DOS) {
        fsname=fsnames[T_VFAT];
        e=mount(devname,mountpoint,fsname,flags|MS_MGC_VAL,NULL);
    }
    if (e) {
	errmsg("failed to mount %s %dK-disk: %s",
	       fsname,F.size/2,strerror(errno));
	goto err_unlock;
    }

#if SETOWNER
    if(fstype != T_DOS && !(flags&MS_RDONLY)) {
	e=chown(mountpoint,ruid,rgid);
	if (e) msg("warning: chown failed");
    }
    if(fstype != T_DOS && !(flags&MS_RDONLY)) {
	e=chmod(mountpoint,~_umask & 0777);
	if (e) msg("warning: chmod failed");
    }
#endif
   
    msg("mounted %s %dK-disk (%s) on %s",fsname,F.size/2,
	(flags&MS_RDONLY) ? "readonly":"read/write", mountpoint);

    /* add entry to /etc/mtab */
    ms.mnt_fsname=devname;
    ms.mnt_dir=mountpoint;
    ms.mnt_type=fsname;
    ms.mnt_opts=options;
    ms.mnt_freq=0;
    ms.mnt_passno=0;
    add_mtab(&ms);

    save_unlock_mtab();
    return 0;

err_close:
    close(fd);
err_unlock:
    save_unlock_mtab();
    return -1;
}
