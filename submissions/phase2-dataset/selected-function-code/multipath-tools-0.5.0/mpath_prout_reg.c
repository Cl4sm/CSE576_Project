int mpath_prout_reg(struct multipath *mpp,int rq_servact, int rq_scope,
	unsigned int rq_type, struct prout_param_descriptor * paramp, int noisy)
{

	int i, j;
	struct pathgroup *pgp = NULL;
	struct path *pp = NULL;
	int rollback = 0;
	int active_pathcount=0;	
	int rc;
	int count=0;
	int status = MPATH_PR_SUCCESS;
	uint64_t sa_key = 0;

	if (!mpp)
		return MPATH_PR_DMMP_ERROR; 

	active_pathcount = pathcount(mpp, PATH_UP) + pathcount(mpp, PATH_GHOST);

	if (active_pathcount == 0) {
		condlog (0, "%s: no path available", mpp->wwid);
		return MPATH_PR_DMMP_ERROR;
	}

	if ( paramp->sa_flags & MPATH_F_ALL_TG_PT_MASK ) {
		condlog (1, "Warning: ALL_TG_PT is set. Configuration not supported");
	}

	struct threadinfo thread[active_pathcount];

	memset(thread, 0, sizeof(thread));

	/* init thread parameter */
	for (i =0; i< active_pathcount; i++){
		thread[i].param.rq_servact = rq_servact;
		thread[i].param.rq_scope = rq_scope;
		thread[i].param.rq_type = rq_type;
		thread[i].param.paramp = paramp;
		thread[i].param.noisy = noisy;
		thread[i].param.status = -1;

		condlog (3, "THRED ID [%d] INFO]", i);
		condlog (3, "rq_servact=%d ", thread[i].param.rq_servact);
		condlog (3, "rq_scope=%d ", thread[i].param.rq_scope); 
		condlog (3, "rq_type=%d ", thread[i].param.rq_type);  
		condlog (3, "rkey="); 
		condlog (3, "paramp->sa_flags =%02x ", thread[i].param.paramp->sa_flags); 
		condlog (3, "noisy=%d ", thread[i].param.noisy); 
		condlog (3, "status=%d ", thread[i].param.status); 
	}

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	vector_foreach_slot (mpp->pg, pgp, j){
		vector_foreach_slot (pgp->paths, pp, i){
			if (!((pp->state == PATH_UP) || (pp->state == PATH_GHOST))){
				condlog (1, "%s: %s path not up. Skip.", mpp->wwid, pp->dev);
				continue;
			}
			strncpy(thread[count].param.dev, pp->dev, FILE_NAME_SIZE);

			if (count && (thread[count].param.paramp->sa_flags & MPATH_F_SPEC_I_PT_MASK)){
				/*
				 * Clearing SPEC_I_PT as transportids are already registered by now.
				 */
				thread[count].param.paramp->sa_flags &= (~MPATH_F_SPEC_I_PT_MASK);
			}

			condlog (3, "%s: sending pr out command to %s", mpp->wwid, pp->dev);

			rc = pthread_create(&thread[count].id, &attr, mpath_prout_pthread_fn, (void *)(&thread[count].param));
			if (rc){
				condlog (0, "%s: failed to create thread %d", mpp->wwid, rc);
			}
			count = count +1;
		}
	}
	for( i=0; i < active_pathcount ; i++){
		rc = pthread_join(thread[i].id, NULL);
		if (rc){
			condlog (0, "%s: Thread[%d] failed to join thread %d", mpp->wwid, i, rc);
		}
		if (!rollback && (thread[i].param.status == MPATH_PR_RESERV_CONFLICT)){
			rollback = 1;
			sa_key = 0;
			for (i = 0; i < 8; ++i){
				if (i > 0)
					sa_key <<= 8;
				sa_key |= paramp->sa_key[i];
			}
			status = MPATH_PR_RESERV_CONFLICT ;
		}
		if (!rollback && (status == MPATH_PR_SUCCESS)){
			status = thread[i].param.status;
		}
	}
	if (rollback && ((rq_servact == MPATH_PROUT_REG_SA) && sa_key != 0 )){
		condlog (3, "%s: ERROR: initiating pr out rollback", mpp->wwid);
		for( i=0 ; i < active_pathcount ; i++){
			if((thread[i].param.status == MPATH_PR_SUCCESS) &&
					((pp->state == PATH_UP) || (pp->state == PATH_GHOST))){
				memcpy(&thread[i].param.paramp->key, &thread[i].param.paramp->sa_key, 8);
				memset(&thread[i].param.paramp->sa_key, 0, 8);
				thread[i].param.status = MPATH_PR_SUCCESS;
				rc = pthread_create(&thread[i].id, &attr, mpath_prout_pthread_fn, 
						(void *)(&thread[count].param));
				if (rc){
					condlog (0, "%s: failed to create thread for rollback. %d",  mpp->wwid, rc);
				}
			}
		}
		for(i=0; i < active_pathcount ; i++){
			rc = pthread_join(thread[i].id, NULL);
			if (rc){
				condlog (3, "%s: failed to join thread while rolling back %d",
						mpp->wwid, i);
			}
		}
	}

	pthread_attr_destroy(&attr);
	return (status);
}
