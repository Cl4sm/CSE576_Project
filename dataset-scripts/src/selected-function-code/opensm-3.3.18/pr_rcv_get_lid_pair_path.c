static osm_sa_item_t *pr_rcv_get_lid_pair_path(IN osm_sa_t * sa,
					       IN const ib_path_rec_t * p_pr,
					       IN const osm_alias_guid_t * p_src_alias_guid,
					       IN const osm_alias_guid_t * p_dest_alias_guid,
					       IN const ib_gid_t * p_sgid,
					       IN const ib_gid_t * p_dgid,
					       IN const uint16_t src_lid_ho,
					       IN const uint16_t dest_lid_ho,
					       IN const ib_net64_t comp_mask,
					       IN const uint8_t preference)
{
	osm_path_parms_t path_parms;
	osm_path_parms_t rev_path_parms;
	osm_sa_item_t *p_pr_item;
	ib_api_status_t status, rev_path_status;

	OSM_LOG_ENTER(sa->p_log);

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Src LID %u, Dest LID %u\n",
		src_lid_ho, dest_lid_ho);

	p_pr_item = malloc(SA_PR_RESP_SIZE);
	if (p_pr_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F01: "
			"Unable to allocate path record\n");
		goto Exit;
	}
	memset(p_pr_item, 0, SA_PR_RESP_SIZE);

	status = pr_rcv_get_path_parms(sa, p_pr, p_src_alias_guid, src_lid_ho,
				       p_dest_alias_guid, dest_lid_ho,
				       comp_mask, &path_parms);

	if (status != IB_SUCCESS) {
		free(p_pr_item);
		p_pr_item = NULL;
		goto Exit;
	}

	/* now try the reversible path */
	rev_path_status = pr_rcv_get_path_parms(sa, p_pr, p_dest_alias_guid,
						dest_lid_ho, p_src_alias_guid,
						src_lid_ho, comp_mask,
						&rev_path_parms);

	path_parms.reversible = (rev_path_status == IB_SUCCESS);

	/* did we get a Reversible Path compmask ? */
	/*
	   NOTE that if the reversible component = 0, it is a don't care
	   rather than requiring non-reversible paths ...
	   see Vol1 Ver1.2 p900 l16
	 */
	if ((comp_mask & IB_PR_COMPMASK_REVERSIBLE) &&
	    !path_parms.reversible && (p_pr->num_path & 0x80)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requested reversible path but failed to get one\n");
		free(p_pr_item);
		p_pr_item = NULL;
		goto Exit;
	}

	pr_rcv_build_pr(sa, p_src_alias_guid, p_dest_alias_guid, p_sgid, p_dgid,
			src_lid_ho, dest_lid_ho, preference, &path_parms,
			&p_pr_item->resp.path_rec);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return p_pr_item;
}