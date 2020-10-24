static void si_rcv_process_new(IN osm_sm_t * sm, IN osm_node_t * p_node,
			       IN const osm_madw_t * p_madw)
{
	osm_switch_t *p_sw;
	osm_switch_t *p_check;
	ib_switch_info_t *p_si;
	ib_smp_t *p_smp;
	cl_qmap_t *p_sw_guid_tbl;

	CL_ASSERT(sm);

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	p_sw_guid_tbl = &sm->p_subn->sw_guid_tbl;
	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_si = ib_smp_get_payload_ptr(p_smp);

	osm_dump_switch_info_v2(sm->p_log, p_si, FILE_ID, OSM_LOG_DEBUG);

	p_sw = osm_switch_new(p_node, p_madw);
	if (p_sw == NULL) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3608: "
			"Unable to allocate new switch object\n");
		goto Exit;
	}

	/* set subnet max mlid to the minimum MulticastFDBCap of all switches */
	if (p_si->mcast_cap &&
	    cl_ntoh16(p_si->mcast_cap) + IB_LID_MCAST_START_HO - 1 <
	    sm->p_subn->max_mcast_lid_ho) {
		sm->p_subn->max_mcast_lid_ho = cl_ntoh16(p_si->mcast_cap) +
			IB_LID_MCAST_START_HO - 1;
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Subnet max multicast lid is 0x%X\n",
			sm->p_subn->max_mcast_lid_ho);
	}

	/* set subnet max unicast lid to the minimum LinearFDBCap of all switches */
	if (cl_ntoh16(p_si->lin_cap) < sm->p_subn->max_ucast_lid_ho) {
		sm->p_subn->max_ucast_lid_ho = cl_ntoh16(p_si->lin_cap);
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Subnet max unicast lid is 0x%X\n",
			sm->p_subn->max_ucast_lid_ho);
	}

	p_check = (osm_switch_t *) cl_qmap_insert(p_sw_guid_tbl,
						  osm_node_get_node_guid
						  (p_node), &p_sw->map_item);
	if (p_check != p_sw) {
		/* This shouldn't happen since we hold the lock! */
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3605: "
			"Unable to add new switch object to database\n");
		osm_switch_delete(&p_sw);
		goto Exit;
	}

	p_node->sw = p_sw;

	/* Update the switch info according to the info we just received. */
	osm_switch_set_switch_info(p_sw, p_si);

#if 0
	/* Don't bother retrieving the current unicast and multicast tables
	   from the switches.  The current version of SM does
	   not support silent take-over of an existing multicast
	   configuration.

	   Gathering the multicast tables can also generate large amounts
	   of extra subnet-init traffic.

	   The code to retrieve the tables was fully debugged. */

	si_rcv_get_fwd_tbl(sm, p_sw);
	if (!sm->p_subn->opt.disable_multicast)
		si_rcv_get_mcast_fwd_tbl(sm, p_sw);
#endif

Exit:
	OSM_LOG_EXIT(sm->p_log);
}