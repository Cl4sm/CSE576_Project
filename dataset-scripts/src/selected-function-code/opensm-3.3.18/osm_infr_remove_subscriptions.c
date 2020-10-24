ib_api_status_t osm_infr_remove_subscriptions(IN osm_subn_t * p_subn,
					      IN osm_log_t * p_log,
					      IN ib_net64_t port_guid)
{
	cl_list_item_t *p_list_item;
	osm_infr_t *p_infr;
	ib_api_status_t status = CL_NOT_FOUND;

	OSM_LOG_ENTER(p_log);

	/* go over all inform info available at the subnet */
	/* match to the given GID and delete subscriptions if match */
	p_list_item = cl_qlist_head(&p_subn->sa_infr_list);
	while (p_list_item != cl_qlist_end(&p_subn->sa_infr_list)) {

		p_infr = (osm_infr_t *)p_list_item;
		p_list_item = cl_qlist_next(p_list_item);

		if (port_guid != p_infr->inform_record.subscriber_gid.unicast.interface_id)
			continue;

		/* Remove this event subscription */
		osm_infr_remove_from_db(p_subn, p_log, p_infr);

		status = CL_SUCCESS;
	}

	OSM_LOG_EXIT(p_log);
	return (status);
}