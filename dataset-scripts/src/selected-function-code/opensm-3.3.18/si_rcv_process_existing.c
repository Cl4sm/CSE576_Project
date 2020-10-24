static boolean_t si_rcv_process_existing(IN osm_sm_t * sm,
					 IN osm_node_t * p_node,
					 IN const osm_madw_t * p_madw)
{
	osm_switch_t *p_sw = p_node->sw;
	ib_switch_info_t *p_si;
	osm_si_context_t *p_si_context;
	ib_smp_t *p_smp;
	osm_epi_lft_change_event_t lft_change;
	boolean_t is_change_detected = FALSE;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_si = ib_smp_get_payload_ptr(p_smp);
	p_si_context = osm_madw_get_si_context_ptr(p_madw);

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG, "Received logical %cetResp()\n",
		p_si_context->set_method ? 'S' : 'G');

	osm_switch_set_switch_info(p_sw, p_si);

	if (p_si_context->light_sweep == TRUE && !p_si_context->set_method) {
		/* If state changed bit is on the mad was returned with an
		   error - signal a change to the state manager. */
		if (ib_smp_get_status(p_smp) != 0) {
			OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
				"GetResp() received with error in light sweep. "
				"Commencing heavy sweep\n");
			is_change_detected = TRUE;
		} else if (ib_switch_info_get_state_change(p_si)) {
			osm_dump_switch_info_v2(sm->p_log, p_si, FILE_ID, OSM_LOG_DEBUG);
			is_change_detected = TRUE;
		}
	}

	if (sm->p_subn->first_time_master_sweep == FALSE &&
	    p_si_context->set_method && p_si_context->lft_top_change) {
		lft_change.p_sw = p_sw;
		lft_change.flags = LFT_CHANGED_LFT_TOP;
		lft_change.lft_top = cl_ntoh16(p_si->lin_top);
		lft_change.block_num = 0;
		osm_opensm_report_event(sm->p_subn->p_osm,
					OSM_EVENT_ID_LFT_CHANGE,
					&lft_change);
	}

	OSM_LOG_EXIT(sm->p_log);
	return is_change_detected;
}