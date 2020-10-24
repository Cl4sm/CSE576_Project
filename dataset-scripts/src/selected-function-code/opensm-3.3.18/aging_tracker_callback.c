static uint64_t aging_tracker_callback(IN uint64_t key, IN uint32_t num_regs,
				       IN void *context)
{
	osm_sm_t *sm = context;
	ib_net16_t lid;
	uint8_t port_num;
	osm_physp_t *p_physp;

	OSM_LOG_ENTER(sm->p_log);

	if (osm_exit_flag)
		/* We got an exit flag - do nothing */
		return 0;

	lid = (ib_net16_t) ((key & 0x0000FFFF00000000ULL) >> 32);
	port_num = (uint8_t) ((key & 0x00FF000000000000ULL) >> 48);

	CL_PLOCK_ACQUIRE(sm->p_lock);

	p_physp = get_physp_by_lid_and_num(sm, lid, port_num);
	if (!p_physp)
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Cannot find port num:%u with lid:%u\n",
			port_num, cl_ntoh16(lid));
	/* make sure the physp is still valid */
	/* If the health port was false - set it to true */
	else if (!osm_physp_is_healthy(p_physp)) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Clearing health bit of port num:%u with lid:%u\n",
			port_num, cl_ntoh16(lid));

		/* Clear its health bit */
		osm_physp_set_health(p_physp, TRUE);
	}

	CL_PLOCK_RELEASE(sm->p_lock);
	OSM_LOG_EXIT(sm->p_log);

	/* We want to remove the event from the tracker - so
	   need to return zero. */
	return 0;
}