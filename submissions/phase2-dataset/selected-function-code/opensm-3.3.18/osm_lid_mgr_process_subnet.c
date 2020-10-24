int osm_lid_mgr_process_subnet(IN osm_lid_mgr_t * p_mgr)
{
	cl_qmap_t *p_port_guid_tbl;
	osm_port_t *p_port;
	ib_net64_t port_guid;
	int lid_changed, ret = 0;
	uint16_t min_lid_ho, max_lid_ho;

	CL_ASSERT(p_mgr);

	OSM_LOG_ENTER(p_mgr->p_log);

	CL_PLOCK_EXCL_ACQUIRE(p_mgr->p_lock);

	CL_ASSERT(p_mgr->p_subn->sm_port_guid);

	p_port_guid_tbl = &p_mgr->p_subn->port_guid_tbl;

	for (p_port = (osm_port_t *) cl_qmap_head(p_port_guid_tbl);
	     p_port != (osm_port_t *) cl_qmap_end(p_port_guid_tbl);
	     p_port = (osm_port_t *) cl_qmap_next(&p_port->map_item)) {
		port_guid = osm_port_get_guid(p_port);

		/*
		   Our own port is a special case in that we want to
		   assign a LID to ourselves first, since we have to
		   advertise that LID value to the other ports.

		   For that reason, our node is treated separately and
		   we will not add it to any of these lists.
		 */
		if (port_guid == p_mgr->p_subn->sm_port_guid) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Skipping our own port 0x%016" PRIx64 "\n",
				cl_ntoh64(port_guid));
			continue;
		}

		/*
		   get the port lid range - we need to send it on first active
		   sweep or if there was a change (the result of
		   lid_mgr_get_port_lid)
		 */
		lid_changed = lid_mgr_get_port_lid(p_mgr, p_port,
						   &min_lid_ho, &max_lid_ho);

		/* we can call the function to update the port info as it known
		   to look for any field change and will only send an updated
		   if required */
		OSM_LOG(p_mgr->p_log, OSM_LOG_VERBOSE,
			"Assigned port 0x%016" PRIx64 ", %s LID [%u,%u]\n",
			cl_ntoh64(port_guid), lid_changed ? "new" : "",
			min_lid_ho, max_lid_ho);

		/* the proc returns the fact it sent a set port info */
		if (lid_mgr_set_physp_pi(p_mgr, p_port, p_port->p_physp,
					 cl_hton16(min_lid_ho)))
			ret = -1;
	}			/* all ports */

	/* store the guid to lid table in persistent db */
	osm_db_store(p_mgr->p_g2l, p_mgr->p_subn->opt.fsync_high_avail_files);

	CL_PLOCK_RELEASE(p_mgr->p_lock);

	OSM_LOG_EXIT(p_mgr->p_log);
	return ret;
}