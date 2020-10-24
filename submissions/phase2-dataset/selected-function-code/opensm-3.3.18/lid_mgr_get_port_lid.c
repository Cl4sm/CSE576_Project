static int lid_mgr_get_port_lid(IN osm_lid_mgr_t * p_mgr,
				IN osm_port_t * p_port,
				OUT uint16_t * p_min_lid,
				OUT uint16_t * p_max_lid)
{
	uint16_t lid, min_lid, max_lid;
	uint64_t guid;
	uint8_t num_lids = (1 << p_mgr->p_subn->opt.lmc);
	int lid_changed = 0;
	uint16_t lmc_mask;

	OSM_LOG_ENTER(p_mgr->p_log);

	/* get the lid from the guid2lid */
	guid = cl_ntoh64(osm_port_get_guid(p_port));

	/* if the port is a base switch port 0 then we only need one lid */
	if (p_port->p_node->sw &&
	    !osm_switch_sp0_is_lmc_capable(p_port->p_node->sw, p_mgr->p_subn))
		num_lids = 1;

	if (p_mgr->p_subn->first_time_master_sweep == TRUE &&
	    p_mgr->p_subn->opt.reassign_lids == TRUE)
		goto AssignLid;

	lmc_mask = ~(num_lids - 1);

	/* if the port matches the guid2lid */
	if (!osm_db_guid2lid_get(p_mgr->p_g2l, guid, &min_lid, &max_lid)) {
		*p_min_lid = min_lid;
		*p_max_lid = min_lid + num_lids - 1;
		if (min_lid == cl_ntoh16(osm_port_get_base_lid(p_port)))
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG, "0x%016" PRIx64
				" matches its known lid:%u\n", guid, min_lid);
		else {
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"0x%016" PRIx64 " with lid:%u "
				"does not match its known lid:%u\n",
				guid, cl_ntoh16(osm_port_get_base_lid(p_port)),
				min_lid);
			lid_mgr_cleanup_discovered_port_lid_range(p_mgr,
								  p_port);
			/* we still need to send the setting to the target port */
			lid_changed = 1;
		}
		goto NewLidSet;
	} else
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"0x%016" PRIx64 " has no persistent lid assigned\n",
			guid);

	/* if the port info carries a lid it must be lmc aligned and not mapped
	   by the persistent storage  */
	min_lid = cl_ntoh16(osm_port_get_base_lid(p_port));

	/* we want to ignore the discovered lid if we are also on first sweep of
	   reassign lids flow */
	if (min_lid) {
		/* make sure lid is valid */
		if ((min_lid & lmc_mask) == min_lid) {
			/* is it free */
			if (lid_mgr_is_range_not_persistent
			    (p_mgr, min_lid, num_lids)) {
				*p_min_lid = min_lid;
				*p_max_lid = min_lid + num_lids - 1;
				OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
					"0x%016" PRIx64
					" lid range:[%u-%u] is free\n",
					guid, *p_min_lid, *p_max_lid);
				goto NewLidSet;
			} else
				OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
					"0x%016" PRIx64 " existing lid "
					"range:[%u:%u] is not free\n",
					guid, min_lid, min_lid + num_lids - 1);
		} else
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"0x%016" PRIx64 " existing lid range:"
				"[%u:%u] is not lmc aligned\n",
				guid, min_lid, min_lid + num_lids - 1);
	}

AssignLid:
	/* first cleanup the existing discovered lid range */
	lid_mgr_cleanup_discovered_port_lid_range(p_mgr, p_port);

	/* find an empty space */
	lid_mgr_find_free_lid_range(p_mgr, num_lids, p_min_lid, p_max_lid);
	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
		"0x%016" PRIx64 " assigned a new lid range:[%u-%u]\n",
		guid, *p_min_lid, *p_max_lid);
	lid_changed = 1;

NewLidSet:
	/* update the guid2lid db and used_lids */
	osm_db_guid2lid_set(p_mgr->p_g2l, guid, *p_min_lid, *p_max_lid);
	for (lid = *p_min_lid; lid <= *p_max_lid; lid++)
		p_mgr->used_lids[lid] = 1;

	/* make sure the assigned lids are marked in port_lid_tbl */
	for (lid = *p_min_lid; lid <= *p_max_lid; lid++)
		cl_ptr_vector_set(&p_mgr->p_subn->port_lid_tbl, lid, p_port);

	OSM_LOG_EXIT(p_mgr->p_log);
	return lid_changed;
}