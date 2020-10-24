    char drive_name[17];
    int e,fd,type;
   
    drive_name[16]=0;
    fd=open(devname,O_RDONLY | O_NDELAY);
    if (fd<=0) return 0;

    e=ioctl(fd,FDGETDRVTYP,(void*)drive_name);
    if (e) {
	errmsg("ioctl(FDGETDRVTYP) failed on %s: %s",devname,strerror(errno));
	close(fd);
	return 0;
    }

    close(fd);

    if (strchr("EDHQdhq",drive_name[0])) {
	type=atoi(drive_name+1);
	if (type==360 || type==720 || type==1200 || 
	    type==1440 || type==2880)
	    {
		return type;
	    }
    }
    errmsg("unknown drive type '%s' for %s",drive_name,devname);
    return 0;
}
