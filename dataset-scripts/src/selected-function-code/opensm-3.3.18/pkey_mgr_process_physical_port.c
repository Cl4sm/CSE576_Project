static void
pkey_mgr_process_physical_port(IN osm_log_t * p_log,
			       IN osm_sm_t * sm,
			       IN const ib_net16_t pkey,
			       IN osm_physp_t * p_physp)
{
	osm_node_t *p_node = osm_physp_get_node_ptr(p_physp);
	osm_pkey_tbl_t *p_pkey_tbl;
	ib_net16_t *p_orig_pkey;
	const char *stat = NULL;
	osm_pending_pkey_t *p_pending;

	p_pkey_tbl = &p_physp->pkeys;
	p_pending = (osm_pending_pkey_t *) malloc(sizeof(osm_pending_pkey_t));
	if (!p_pending) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0502: "
			"Failed to allocate new pending pkey entry for node "
			"0x%016" PRIx64 " port %u\n",
			cl_ntoh64(osm_node_get_node_guid(p_node)),
			osm_physp_get_port_num(p_physp));
		return;
	}
	p_pending->pkey = pkey;
	if (sm->p_subn->opt.allow_both_pkeys)
		p_orig_pkey = cl_map_get(&p_pkey_tbl->keys, pkey);
	else
		p_orig_pkey = cl_map_get(&p_pkey_tbl->keys,
					 ib_pkey_get_base(pkey));
	if (!p_orig_pkey) {
		p_pending->is_new = TRUE;
		cl_qlist_insert_tail(&p_pkey_tbl->pending,
				     (cl_list_item_t *) p_pending);
		stat = "inserted";
	} else {
		CL_ASSERT(ib_pkey_get_base(*p_orig_pkey) ==
			  ib_pkey_get_base(pkey));
		p_pending->is_new = FALSE;
		if (osm_pkey_tbl_get_block_and_idx(p_pkey_tbl, p_orig_pkey,
						   &p_pending->block,
						   &p_pending->index) !=
		    IB_SUCCESS) {
			OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0503: "
				"Failed to obtain P_Key 0x%04x block and index "
				"for node 0x%016" PRIx64 " port %u\n",
				cl_ntoh16(ib_pkey_get_base(pkey)),
				cl_ntoh64(osm_node_get_node_guid(p_node)),
				osm_physp_get_port_num(p_physp));
			free(p_pending);
			return;
		}
		cl_qlist_insert_head(&p_pkey_tbl->pending,
				     (cl_list_item_t *) p_pending);
		stat = "updated";
	}

	OSM_LOG(p_log, OSM_LOG_DEBUG,
		"pkey 0x%04x was %s for node 0x%016" PRIx64 " port %u\n",
		cl_ntoh16(pkey), stat,
		cl_ntoh64(osm_node_get_node_guid(p_node)),
		osm_physp_get_port_num(p_physp));
}