static ib_api_status_t cc_send_cct(osm_sm_t * p_sm,
				   osm_node_t *p_node,
				   osm_physp_t *p_physp)
{
	osm_congestion_control_t *p_cc = &p_sm->p_subn->p_osm->cc;
	unsigned force_update;
	osm_madw_t *p_madw = NULL;
	ib_cc_mad_t *p_cc_mad = NULL;
	ib_cc_tbl_t *p_cc_tbl = NULL;
	unsigned int index = 0;

	OSM_LOG_ENTER(p_sm->p_log);

	force_update = p_physp->need_update || p_sm->p_subn->need_update;

	for (index = 0; index < p_cc->cc_tbl_mads; index++) {
		if (!force_update
		    && !memcmp(&p_cc->cc_tbl[index],
			       &p_physp->cc.ca.cc_tbl[index],
			       sizeof(p_cc->cc_tbl[index])))
			continue;

		p_madw = osm_mad_pool_get(p_cc->mad_pool, p_cc->bind_handle,
					  MAD_BLOCK_SIZE, NULL);
		if (p_madw == NULL) {
			OSM_LOG(p_sm->p_log, OSM_LOG_ERROR, "ERR C103: "
				"failed to allocate mad\n");
			return IB_INSUFFICIENT_MEMORY;
		}

		p_cc_mad = osm_madw_get_cc_mad_ptr(p_madw);

		p_cc_tbl = (ib_cc_tbl_t *)ib_cc_mad_get_mgt_data_ptr(p_cc_mad);

		memcpy(p_cc_tbl,
		       &p_cc->cc_tbl[index],
		       sizeof(p_cc->cc_tbl[index]));

		cc_mad_post(p_cc, p_madw, p_node, p_physp,
			    IB_MAD_ATTR_CC_TBL, cl_hton32(index));
	}

	OSM_LOG_EXIT(p_sm->p_log);

	return IB_SUCCESS;
}