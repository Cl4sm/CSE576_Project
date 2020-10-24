mpath_prin_activepath (struct multipath *mpp, int rq_servact,
	struct prin_resp * resp, int noisy)
{
	int i,j, ret = MPATH_PR_DMMP_ERROR;
	struct pathgroup *pgp = NULL;
	struct path *pp = NULL;

	vector_foreach_slot (mpp->pg, pgp, j){
		vector_foreach_slot (pgp->paths, pp, i){
			if (!((pp->state == PATH_UP) || (pp->state == PATH_GHOST))){
				condlog(2, "%s: %s not available. Skip.", mpp->wwid, pp->dev);
				condlog(3, "%s: status = %d.", mpp->wwid, pp->state);
				continue;
			}

			condlog(3, "%s: sending pr in command to %s ", mpp->wwid, pp->dev);
			ret = mpath_send_prin_activepath(pp->dev, rq_servact, resp, noisy);
			switch(ret)
			{
				case MPATH_PR_SUCCESS:
				case MPATH_PR_SENSE_INVALID_OP:
					return ret;
				default:
					continue;
			}
		}
	}
	return ret;	
}
