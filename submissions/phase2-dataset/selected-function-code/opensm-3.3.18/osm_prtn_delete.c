void osm_prtn_delete(IN osm_subn_t * p_subn, IN OUT osm_prtn_t ** pp_prtn)
{
	char gid_str[INET6_ADDRSTRLEN];
	int i = 0;
	osm_prtn_t *p = *pp_prtn;

	cl_map_remove_all(&p->full_guid_tbl);
	cl_map_destroy(&p->full_guid_tbl);
	cl_map_remove_all(&p->part_guid_tbl);
	cl_map_destroy(&p->part_guid_tbl);

	if (p->mgrps) {
		/* Clean up mgrps */
		for (i = 0; i < p->nmgrps; i++) {
			/* osm_mgrp_cleanup will not delete
			 * "well_known" groups */
			p->mgrps[i]->well_known = FALSE;
			OSM_LOG(&p_subn->p_osm->log, OSM_LOG_DEBUG,
				"removing mgroup %s from partition (0x%x)\n",
				inet_ntop(AF_INET6,
					  p->mgrps[i]->mcmember_rec.mgid.raw,
					  gid_str, sizeof gid_str),
				cl_hton16(p->pkey));
			osm_mgrp_cleanup(p_subn, p->mgrps[i]);
		}

		free(p->mgrps);
	}

	free(p);
	*pp_prtn = NULL;
}