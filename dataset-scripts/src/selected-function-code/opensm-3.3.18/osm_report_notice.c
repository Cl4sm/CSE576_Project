ib_api_status_t osm_report_notice(IN osm_log_t * p_log, IN osm_subn_t * p_subn,
				  IN ib_mad_notice_attr_t * p_ntc)
{
	osm_infr_match_ctxt_t context;
	cl_list_t infr_to_remove_list;
	osm_infr_t *p_infr_rec;
	osm_infr_t *p_next_infr_rec;

	OSM_LOG_ENTER(p_log);

	/*
	 * we must make sure we are ready for this...
	 * note that the trap receivers might be initialized before
	 * the osm_infr_init call is performed.
	 */
	if (p_subn->sa_infr_list.state != CL_INITIALIZED) {
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Ignoring Notice Reports since Inform List is not initialized yet!\n");
		return IB_ERROR;
	}

	if (OSM_LOG_IS_ACTIVE_V2(p_log, OSM_LOG_INFO))
		log_notice(p_log, OSM_LOG_INFO, p_ntc);

	/* Create a list that will hold all the infr records that should
	   be removed due to violation. o13-17.1.2 */
	cl_list_construct(&infr_to_remove_list);
	cl_list_init(&infr_to_remove_list, 5);
	context.p_remove_infr_list = &infr_to_remove_list;
	context.p_ntc = p_ntc;

	/* go over all inform info available at the subnet */
	/* try match to the given notice and send if match */
	cl_qlist_apply_func(&p_subn->sa_infr_list, match_notice_to_inf_rec,
			    &context);

	/* If we inserted items into the infr_to_remove_list - we need to
	   remove them */
	p_infr_rec = (osm_infr_t *) cl_list_remove_head(&infr_to_remove_list);
	while (p_infr_rec != NULL) {
		p_next_infr_rec =
		    (osm_infr_t *) cl_list_remove_head(&infr_to_remove_list);
		osm_infr_remove_from_db(p_subn, p_log, p_infr_rec);
		p_infr_rec = p_next_infr_rec;
	}
	cl_list_destroy(&infr_to_remove_list);

	/* report IB traps to plugin */
	osm_opensm_report_event(p_subn->p_osm, OSM_EVENT_ID_TRAP, p_ntc);

	OSM_LOG_EXIT(p_log);

	return IB_SUCCESS;
}