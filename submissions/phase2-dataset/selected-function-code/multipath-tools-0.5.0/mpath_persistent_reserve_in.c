{
	struct stat info;
	vector curmp = NULL;
	vector pathvec = NULL;
	char * alias;
	struct multipath * mpp;
	int map_present;
	int major, minor;
	int ret;

	conf->verbosity = verbose;

	if (fstat( fd, &info) != 0){
		condlog(0, "stat error %d", fd);
		return MPATH_PR_FILE_ERROR;
	} 
	if(!S_ISBLK(info.st_mode)){
		condlog(0, "Failed to get major:minor. fd = %d", fd);
		return MPATH_PR_FILE_ERROR;
	}

	major = (int)MAJOR(info.st_rdev);
	minor = (int)MINOR(info.st_rdev);	
	condlog(4, "Device %d:%d:  ", major, minor);

	/* get alias from major:minor*/
	alias = dm_mapname(major, minor);
	if (!alias){
		condlog(0, "%d:%d failed to get device alias.", major, minor);
		return MPATH_PR_DMMP_ERROR;
	}

	condlog(3, "alias = %s", alias);
	map_present = dm_map_present(alias);
	if (map_present && dm_type(alias, TGT_MPATH) <= 0){
		condlog( 0, "%s: not a multipath device.", alias);
		ret = MPATH_PR_DMMP_ERROR;
		goto out;
	}

	/*
	 * allocate core vectors to store paths and multipaths
	 */
	curmp = vector_alloc ();
	pathvec = vector_alloc ();

	if (!curmp || !pathvec){
		condlog (0, "%s: vector allocation failed.", alias);
		ret = MPATH_PR_DMMP_ERROR;
		goto out;
	}

	if (path_discovery(pathvec, conf, DI_SYSFS | DI_CHECKER)) {
		ret = MPATH_PR_DMMP_ERROR;
		goto out1;
	}

	/* get info of all paths from the dm device	*/
	if (get_mpvec (curmp, pathvec, alias)){
		condlog(0, "%s: failed to get device info.", alias);
		ret = MPATH_PR_DMMP_ERROR;
		goto out1;
	}

	mpp = find_mp_by_alias(curmp, alias);
	if (!mpp){
		condlog(0, "%s: devmap not registered.", alias);
		ret = MPATH_PR_DMMP_ERROR;
		goto out1;
	}

	ret = mpath_prin_activepath(mpp, rq_servact, resp, noisy);

out1:
	free_multipathvec(curmp, KEEP_PATHS);
	free_pathvec(pathvec, FREE_PATHS);	
out:
	FREE(alias);
	return ret; 						
}
