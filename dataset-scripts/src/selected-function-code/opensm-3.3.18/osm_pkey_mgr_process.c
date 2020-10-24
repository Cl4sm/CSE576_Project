int osm_pkey_mgr_process(IN osm_opensm_t * p_osm)
{
	cl_qmap_t *p_tbl;
	cl_map_item_t *p_next;
	osm_prtn_t *p_prtn;
	osm_port_t *p_port;
	osm_switch_t *p_sw;
	osm_physp_t *p_physp;
	osm_node_t *p_remote_node;
	uint8_t i;
	int ret = 0;

	CL_ASSERT(p_osm);

	OSM_LOG_ENTER(&p_osm->log);

	CL_PLOCK_EXCL_ACQUIRE(&p_osm->lock);

	if (osm_prtn_make_partitions(&p_osm->log, &p_osm->subn) != IB_SUCCESS) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR, "ERR 0510: "
			"osm_prtn_make_partitions() failed\n");
		ret = -1;
		goto _err;
	}

	/* populate the pending pkey entries by scanning all partitions */
	p_tbl = &p_osm->subn.prtn_pkey_tbl;
	p_next = cl_qmap_head(p_tbl);
	while (p_next != cl_qmap_end(p_tbl)) {
		p_prtn = (osm_prtn_t *) p_next;
		p_next = cl_qmap_next(p_next);
		pkey_mgr_process_partition_table(&p_osm->log, &p_osm->sm,
						 p_prtn, FALSE);
		pkey_mgr_process_partition_table(&p_osm->log, &p_osm->sm,
						 p_prtn, TRUE);
	}

	/* calculate and set new pkey tables */
	p_tbl = &p_osm->subn.port_guid_tbl;
	p_next = cl_qmap_head(p_tbl);
	while (p_next != cl_qmap_end(p_tbl)) {
		p_port = (osm_port_t *) p_next;
		p_next = cl_qmap_next(p_next);
		if (pkey_mgr_update_port(&p_osm->log, &p_osm->sm, p_port))
			ret = -1;
		if ((osm_node_get_type(p_port->p_node) != IB_NODE_TYPE_SWITCH)
		    && pkey_mgr_update_peer_port(&p_osm->log, &p_osm->sm,
						 &p_osm->subn, p_port,
						 p_osm->subn.opt.part_enforce_enum))
			ret = -1;
	}

	/* clear partition enforcement on inter-switch links */
	p_tbl = &p_osm->subn.sw_guid_tbl;
	p_next = cl_qmap_head(p_tbl);
	while (p_next != cl_qmap_end(p_tbl)) {
		p_sw = (osm_switch_t *) p_next;
		p_next = cl_qmap_next(p_next);
		for (i = 1; i < p_sw->num_ports; i++) {
			p_physp = osm_node_get_physp_ptr(p_sw->p_node, i);
			if (p_physp && p_physp->p_remote_physp)
				p_remote_node = p_physp->p_remote_physp->p_node;
			else
				continue;

			if (osm_node_get_type(p_remote_node) != IB_NODE_TYPE_SWITCH)
				continue;

			if(! (p_physp->port_info.vl_enforce & 0xc ))
				continue;

			/* clear partition enforcement */
			if (pkey_mgr_enforce_partition(&p_osm->log, &p_osm->sm, p_physp, OSM_PARTITION_ENFORCE_TYPE_OFF))
				ret = -1;
		}
	}
_err:
	CL_PLOCK_RELEASE(&p_osm->lock);
	OSM_LOG_EXIT(&p_osm->log);
	return ret;
}