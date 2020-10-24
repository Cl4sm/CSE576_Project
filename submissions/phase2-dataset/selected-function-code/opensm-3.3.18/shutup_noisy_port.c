static int shutup_noisy_port(osm_sm_t *sm, ib_net16_t lid, uint8_t port,
			     unsigned num)
{
	osm_physp_t *p = get_physp_by_lid_and_num(sm, lid, port);
	if (!p) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3805: "
			"Failed to find physical port by lid:%u num:%u\n",
			cl_ntoh16(lid), port);
		return -1;
	}

	/* When babbling port policy option is enabled and
	   Threshold for disabling a "babbling" port is exceeded */
	if (sm->p_subn->opt.babbling_port_policy && num >= 250) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Disabling noisy physical port 0x%016" PRIx64
			": lid %u, num %u\n",
			cl_ntoh64(osm_physp_get_port_guid(p)),
			cl_ntoh16(lid), port);
		if (disable_port(sm, p))
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3811: "
				"Failed to disable noisy physical port 0x%016"
				PRIx64 ": lid %u, num %u\n",
				cl_ntoh64(osm_physp_get_port_guid(p)),
				cl_ntoh16(lid), port);
		else
			return 1;
	}

	/* check if the current state of the p_physp is healthy. If
	   it is - then this is a first change of state. Run a heavy sweep. */
	if (osm_physp_is_healthy(p)) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Marking unhealthy physical port by lid:%u num:%u\n",
			cl_ntoh16(lid), port);
		osm_physp_set_health(p, FALSE);
		return 2;
	}
	return 0;
}