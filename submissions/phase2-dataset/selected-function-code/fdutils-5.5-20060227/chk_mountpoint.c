    int must_own;
    struct stat st;

    if (stat(dir,&st)!=0) {
	errmsg("Can't access %s: %s\n",dir,strerror(errno));
	return 0;
    }

    if (!S_ISDIR(st.st_mode)) {
	errmsg("%s is not a directory\n",dir);
	return 0;
    }

    if (!is_default && access(dir,W_OK)!=0) {
	errmsg("No write permission to %s\n",dir);
	return 0;
    }

#if CHKOWNER_MOUNT
    /* user specified mount points must be owned by the user
       unless the user is root
       */
    must_own=(ruid!=0) && !is_default;
    if (must_own && st.st_uid!=ruid) {
	errmsg("Not owner of %s\n",dir);
	return 0;
    }
#endif

    return 1;
}
