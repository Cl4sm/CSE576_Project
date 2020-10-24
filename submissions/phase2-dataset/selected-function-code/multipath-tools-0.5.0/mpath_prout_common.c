        unsigned int rq_type, struct prout_param_descriptor* paramp, int noisy)
{
	int i,j, ret;
	struct pathgroup *pgp = NULL;
	struct path *pp = NULL;

	vector_foreach_slot (mpp->pg, pgp, j){
		vector_foreach_slot (pgp->paths, pp, i){
			if (!((pp->state == PATH_UP) || (pp->state == PATH_GHOST))){
				condlog (1, "%s: %s path not up. Skip", mpp->wwid, pp->dev); 
				continue;
			}

			condlog (3, "%s: sending pr out command to %s", mpp->wwid, pp->dev);
			ret = send_prout_activepath(pp->dev, rq_servact, rq_scope, rq_type, 
					paramp, noisy); 
			return ret ;
		}
	}
	return MPATH_PR_SUCCESS;
}
