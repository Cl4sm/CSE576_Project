get_mpvec (vector curmp, vector pathvec, char * refwwid)
{
	int i;
	struct multipath *mpp;
	char params[PARAMS_SIZE], status[PARAMS_SIZE];

	if (dm_get_maps (curmp)){
		return 1;
	}

	vector_foreach_slot (curmp, mpp, i){
		/*
		 * discard out of scope maps
		 */
		if (mpp->alias && refwwid && strncmp (mpp->alias, refwwid, WWID_SIZE)){
			free_multipath (mpp, KEEP_PATHS);
			vector_del_slot (curmp, i);
			i--;
			continue;
		}

		dm_get_map(mpp->alias, &mpp->size, params);
		condlog(3, "params = %s", params);
		dm_get_status(mpp->alias, status);
                condlog(3, "status = %s", status);
		disassemble_map (pathvec, params, mpp);
		
		/*
		 * disassemble_map() can add new paths to pathvec.
		 * If not in "fast list mode", we need to fetch information
		 * about them
		 */
		updatepaths(mpp);
		mpp->bestpg = select_path_group (mpp);
		disassemble_status (status, mpp);

	}
	return MPATH_PR_SUCCESS ;
}
