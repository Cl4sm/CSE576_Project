static ib_api_status_t
track_mgrp_w_partition(osm_log_t *p_log, osm_prtn_t *p, osm_mgrp_t *mgrp,
			osm_subn_t *p_subn, const ib_gid_t *mgid,
			ib_net16_t pkey)
{
	char gid_str[INET6_ADDRSTRLEN];
	osm_mgrp_t **tmp;
	int i = 0;

	/* check if we are already tracking this group */
	for (i = 0; i< p->nmgrps; i++)
		if (p->mgrps[i] == mgrp)
			return (IB_SUCCESS);

	/* otherwise add it to our list */
	tmp = realloc(p->mgrps, (p->nmgrps +1) * sizeof(*p->mgrps));
	if (tmp) {
		p->mgrps = tmp;
		p->mgrps[p->nmgrps] = mgrp;
		p->nmgrps++;
	} else {
		OSM_LOG(p_log, OSM_LOG_ERROR,
			"realloc error to create MC group (%s) in "
			"partition (pkey 0x%04x)\n",
			inet_ntop(AF_INET6, mgid->raw,
				  gid_str, sizeof gid_str),
			cl_ntoh16(pkey));
		mgrp->well_known = FALSE;
		osm_mgrp_cleanup(p_subn, mgrp);
		return (IB_ERROR);
	}
	mgrp->well_known = TRUE;
	return (IB_SUCCESS);
}