static int lid_mgr_process_our_sm_node(IN osm_lid_mgr_t * p_mgr)
{
	osm_port_t *p_port;
	uint16_t min_lid_ho;
	uint16_t max_lid_ho;
	int ret;

	OSM_LOG_ENTER(p_mgr->p_log);

	/*
	   Acquire our own port object.
	 */
	p_port = osm_get_port_by_guid(p_mgr->p_subn,
				      p_mgr->p_subn->sm_port_guid);
	if (!p_port) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 0308: "
			"Can't acquire SM's port object, GUID 0x%016" PRIx64
			"\n", cl_ntoh64(p_mgr->p_subn->sm_port_guid));
		ret = -1;
		goto Exit;
	}

	/*
	   Determine the LID this SM will use for its own port.
	   Be careful.  With an LMC > 0, the bottom of the LID range becomes
	   unusable, since port hardware will mask off least significant bits,
	   leaving a LID of 0 (invalid).  Therefore, make sure that we always
	   configure the SM with a LID that has non-zero bits, even after
	   LMC masking by hardware.
	 */
	lid_mgr_get_port_lid(p_mgr, p_port, &min_lid_ho, &max_lid_ho);
	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
		"Current base LID is %u\n", min_lid_ho);
	/*
	   Update subnet object.
	 */
	p_mgr->p_subn->master_sm_base_lid = cl_hton16(min_lid_ho);
	p_mgr->p_subn->sm_base_lid = cl_hton16(min_lid_ho);

	OSM_LOG(p_mgr->p_log, OSM_LOG_VERBOSE,
		"Assigning SM's port 0x%016" PRIx64
		"\n\t\t\t\tto LID range [%u,%u]\n",
		cl_ntoh64(osm_port_get_guid(p_port)), min_lid_ho, max_lid_ho);

	/*
	   Set the PortInfo the Physical Port associated with this Port.
	 */
	ret = lid_mgr_set_physp_pi(p_mgr, p_port, p_port->p_physp,
				   cl_hton16(min_lid_ho));

Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
	return ret;
}