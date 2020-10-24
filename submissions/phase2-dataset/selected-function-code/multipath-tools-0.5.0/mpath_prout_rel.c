int mpath_prout_rel(struct multipath *mpp,int rq_servact, int rq_scope,
        unsigned int rq_type, struct prout_param_descriptor * paramp, int noisy)
{
	int i, j;
	int num = 0;
	struct pathgroup *pgp = NULL;
	struct path *pp = NULL;
	int active_pathcount = 0;
	pthread_attr_t attr;
	int rc, found = 0;;
	int count = 0;
	int status = MPATH_PR_SUCCESS;
	struct prin_resp resp;
	struct prout_param_descriptor *pamp;
	struct prin_resp *pr_buff;
	int length;
	struct transportid *pptr;	

	if (!mpp)
		return MPATH_PR_DMMP_ERROR;

	active_pathcount = pathcount (mpp, PATH_UP) + pathcount (mpp, PATH_GHOST);

	struct threadinfo thread[active_pathcount];
	memset(thread, 0, sizeof(thread));
	for (i = 0; i < active_pathcount; i++){
		thread[i].param.rq_servact = rq_servact;
		thread[i].param.rq_scope = rq_scope;
		thread[i].param.rq_type = rq_type;
		thread[i].param.paramp = paramp;
		thread[i].param.noisy = noisy;
		thread[i].param.status = -1;

		condlog (3, " path count = %d", i);
		condlog (3, "rq_servact=%d ", thread[i].param.rq_servact);
		condlog (3, "rq_scope=%d ", thread[i].param.rq_scope);
		condlog (3, "rq_type=%d ", thread[i].param.rq_type);
		condlog (3, "noisy=%d ", thread[i].param.noisy);
		condlog (3, "status=%d ", thread[i].param.status);
	}

	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);

	vector_foreach_slot (mpp->pg, pgp, j){
		vector_foreach_slot (pgp->paths, pp, i){
			if (!((pp->state == PATH_UP) || (pp->state == PATH_GHOST))){
				condlog (1, "%s: %s path not up.", mpp->wwid, pp->dev);
				continue;
			}
			
			strncpy(thread[count].param.dev, pp->dev, FILE_NAME_SIZE);
			condlog (3, "%s: sending pr out command to %s", mpp->wwid, pp->dev);
			rc = pthread_create (&thread[count].id, &attr, mpath_prout_pthread_fn,
					(void *) (&thread[count].param));
			if (rc)
				condlog (0, "%s: failed to create thread. %d",  mpp->wwid, rc);
			count = count + 1;
		}
	}
	pthread_attr_destroy (&attr);
	for (i = 0; i < active_pathcount; i++){
		rc = pthread_join (thread[i].id, NULL);
		if (rc){
			condlog (1, "%s: failed to join thread.  %d",  mpp->wwid,  rc);
		}
	}

	for (i = 0; i < active_pathcount; i++){
		/*  check thread status here and return the status */

		if (thread[i].param.status == MPATH_PR_RESERV_CONFLICT)
			status = MPATH_PR_RESERV_CONFLICT;
		else if (status == MPATH_PR_SUCCESS
				&& thread[i].param.status != MPATH_PR_RESERV_CONFLICT)
			status = thread[i].param.status;
	}

	status = mpath_prin_activepath (mpp, MPATH_PRIN_RRES_SA, &resp, noisy);
	if (status != MPATH_PR_SUCCESS){
		condlog (0, "%s: pr in read reservation command failed.", mpp->wwid);
		return MPATH_PR_OTHER;
	}

	num = resp.prin_descriptor.prin_readresv.additional_length / 8;
	if (num == 0){
		condlog (2, "%s: Path holding reservation is released.", mpp->wwid);
		return MPATH_PR_SUCCESS;	
	}
	condlog (2, "%s: Path holding reservation is not avialable.", mpp->wwid);

	pr_buff =  mpath_alloc_prin_response(MPATH_PRIN_RFSTAT_SA);
	if (!pr_buff){
		condlog (0, "%s: failed to  alloc pr in response buffer.", mpp->wwid);	
		return MPATH_PR_OTHER;
	}

	status = mpath_prin_activepath (mpp, MPATH_PRIN_RFSTAT_SA, pr_buff, noisy);

	if (status != MPATH_PR_SUCCESS){
		condlog (0,  "%s: pr in read full status command failed.",  mpp->wwid);
		goto out;
	}

	num = pr_buff->prin_descriptor.prin_readfd.number_of_descriptor;
	if (0 == num){
		goto out;
	}
	length = sizeof (struct prout_param_descriptor) + (sizeof (struct transportid *));

	pamp = (struct prout_param_descriptor *)malloc (length);
	if (!pamp){
		condlog (0, "%s: failed to alloc pr out parameter.", mpp->wwid);
		goto out1;
	}

	memset(pamp, 0, length);

	pamp->trnptid_list[0] = (struct transportid *) malloc (sizeof (struct transportid));
	if (!pamp->trnptid_list[0]){
		condlog (0, "%s: failed to alloc pr out transportid.", mpp->wwid);
		goto out1;
	}

	if (mpp->reservation_key ){
		memcpy (pamp->key, mpp->reservation_key, 8);
		condlog (3, "%s: reservation key set.", mpp->wwid);
	}

	mpath_prout_common (mpp, MPATH_PROUT_CLEAR_SA, rq_scope, rq_type, pamp,
			noisy);

	pamp->num_transportid = 1;
	pptr=pamp->trnptid_list[0];

	for (i = 0; i < num; i++){
		if (mpp->reservation_key && 
			memcmp(pr_buff->prin_descriptor.prin_readfd.descriptors[i]->key,
			mpp->reservation_key, 8)){	
			/*register with tarnsport id*/
			memset(pamp, 0, length);
			pamp->trnptid_list[0] = pptr;
			memset (pamp->trnptid_list[0], 0, sizeof (struct transportid));
			memcpy (pamp->sa_key,
					pr_buff->prin_descriptor.prin_readfd.descriptors[i]->key, 8);
			pamp->sa_flags = MPATH_F_SPEC_I_PT_MASK;
			pamp->num_transportid = 1;

			memcpy (pamp->trnptid_list[0],
					&pr_buff->prin_descriptor.prin_readfd.descriptors[i]->trnptid,
					sizeof (struct transportid));
			status = mpath_prout_common (mpp, MPATH_PROUT_REG_SA, 0, rq_type,
					pamp, noisy);

			pamp->sa_flags = 0;
			memcpy (pamp->key, pr_buff->prin_descriptor.prin_readfd.descriptors[i]->key, 8);
			memset (pamp->sa_key, 0, 8);
			pamp->num_transportid = 0;
			status = mpath_prout_common (mpp, MPATH_PROUT_REG_SA, 0, rq_type,
					pamp, noisy);
		}
		else
		{
			if (mpp->reservation_key)
				found = 1;
		}


	}

	if (found){
		memset (pamp, 0, length);
		memcpy (pamp->sa_key, mpp->reservation_key, 8);
		memset (pamp->key, 0, 8);
		status = mpath_prout_reg(mpp, MPATH_PROUT_REG_SA, rq_scope, rq_type, pamp, noisy);	
	}


	free(pptr);
out1:
	free (pamp);
out:
	free (pr_buff);
	return (status);
}
