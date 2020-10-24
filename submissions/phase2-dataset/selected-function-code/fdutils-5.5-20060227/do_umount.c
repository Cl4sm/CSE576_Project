    int e,fuid;
    mnt_node *mnt;
    char *mountpoint, *uidstr;
    struct stat st;

    lock_read_mtab();
    strncpy(curdev,devname, sizeof(curdev));
    curdev[sizeof(curdev)-1]='\0';


    mnt=get_mounted(devname);
    if (!mnt) {
	errmsg("not mounted");
	save_unlock_mtab();
	return -1;
    }
    else {
	mountpoint=mnt->ms.mnt_dir;

#if CHKOWNER_UMOUNT
	e=stat(mountpoint,&st);
	if (e) {
	    errmsg("Cannot access %s: %s\n",mountpoint,strerror(errno));
	    goto err;
	}
	uidstr = strstr(mnt->ms.mnt_opts, ",uid=");
	if(uidstr)
	    fuid = atoi(uidstr+5);
	else {
	    uidstr = strstr(mnt->ms.mnt_opts, ",resuid=");
	    if(uidstr)
		fuid = atoi(uidstr+8);
	    fuid = 0;
	}
	if (ruid && st.st_uid!=ruid && fuid != ruid) {
	    errmsg("Not owner of mounted directory: UID=%d\n",st.st_uid);
	    goto err;
	}
    }
#endif
   
    e=umount(mountpoint);
    if (e) {
	errmsg("failed to unmount: %s\n",strerror(errno));
	goto err;
    }
    remove_mtab(devname);

#if 0
    /* have to check whether user's own for this. */
    chown(mountpoint,0,0);   /* back to root */
    chmod(mountpoint,0700);  /* no public permissions */
#endif

    msg("disk unmounted");

    save_unlock_mtab();
    return 0;

err:
    save_unlock_mtab();
    return -1;
}
