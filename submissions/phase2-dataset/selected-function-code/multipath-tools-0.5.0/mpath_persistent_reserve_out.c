int mpath_persistent_reserve_out ( int fd, int rq_servact, int rq_scope,
		unsigned int rq_type, struct prout_param_descriptor *paramp, int noisy, int verbose)
{

	struct stat info;

	vector curmp = NULL;
	vector pathvec = NULL;

	char * alias;
	struct multipath * mpp;
	int map_present;
	int major, minor;
	int ret;
	int j;
	unsigned char *keyp;
	uint64_t prkey;		

	conf->verbosity = verbose;

	if (fstat( fd, &info) != 0){
		condlog(0, "stat error fd=%d", fd);
		return MPATH_PR_FILE_ERROR;
	}

	if(!S_ISBLK(info.st_mode)){
		condlog(3, "Failed to get major:minor. fd=%d", fd);
		return MPATH_PR_FILE_ERROR;	
	}	

	major = (int)MAJOR(info.st_rdev);
	minor = (int)MINOR(info.st_rdev);
	condlog(4, "Device  %d:%d", major, minor);

	/* get WWN of the device from major:minor*/
	alias = dm_mapname(major, minor);
	if (!alias){
		return MPATH_PR_DMMP_ERROR;
	}

	condlog(3, "alias = %s", alias);
	map_present = dm_map_present(alias);

	if (map_present && dm_type(alias, TGT_MPATH) <= 0){
		condlog(3, "%s: not a multipath device.", alias);
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

	/* get info of all paths from the dm device     */
	if (get_mpvec(curmp, pathvec, alias)){
		condlog(0, "%s: failed to get device info.", alias);
		ret = MPATH_PR_DMMP_ERROR;
		goto out1;
	}

	mpp = find_mp_by_alias(curmp, alias);

	if (!mpp) {
		condlog(0, "%s: devmap not registered.", alias);
		ret = MPATH_PR_DMMP_ERROR;
		goto out1;
	}

	select_reservation_key(mpp);

	switch(rq_servact)
	{
		case MPATH_PROUT_REG_SA: 
		case MPATH_PROUT_REG_IGN_SA:  
			ret= mpath_prout_reg(mpp, rq_servact, rq_scope, rq_type, paramp, noisy);
			break;
		case MPATH_PROUT_RES_SA :  
		case MPATH_PROUT_PREE_SA :  
		case MPATH_PROUT_PREE_AB_SA :  
		case MPATH_PROUT_CLEAR_SA:  
			ret = mpath_prout_common(mpp, rq_servact, rq_scope, rq_type, paramp, noisy);
			break;
		case MPATH_PROUT_REL_SA:
			ret = mpath_prout_rel(mpp, rq_servact, rq_scope, rq_type, paramp, noisy);
			break;
		default:
			ret = MPATH_PR_OTHER;
			goto out1;
	}

	if ((ret == MPATH_PR_SUCCESS) && ((rq_servact == MPATH_PROUT_REG_SA) ||
				(rq_servact ==  MPATH_PROUT_REG_IGN_SA)))
	{
		keyp=paramp->sa_key;
		prkey = 0;
		for (j = 0; j < 8; ++j) {
			if (j > 0)
				prkey <<= 8;
			prkey |= *keyp;
			++keyp;
		}
		if (prkey == 0)
			update_prflag(alias, "unset", noisy);
		else
			update_prflag(alias, "set", noisy);
	} else {
		if ((ret == MPATH_PR_SUCCESS) && ((rq_servact == MPATH_PROUT_CLEAR_SA) || 
					(rq_servact == MPATH_PROUT_PREE_AB_SA ))){
			update_prflag(alias, "unset", noisy);
		}
	}
out1:
	free_multipathvec(curmp, KEEP_PATHS);
	free_pathvec(pathvec, FREE_PATHS);

out:
	FREE(alias);
	return ret; 
}
