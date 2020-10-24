osm_infr_t *osm_infr_get_by_rec(IN osm_subn_t const *p_subn,
				IN osm_log_t * p_log,
				IN osm_infr_t * p_infr_rec)
{
	cl_list_item_t *p_list_item;

	OSM_LOG_ENTER(p_log);

	dump_all_informs(p_subn, p_log);

	OSM_LOG(p_log, OSM_LOG_DEBUG, "Looking for Inform Record\n");
	osm_dump_inform_info_v2(p_log, &(p_infr_rec->inform_record.inform_info),
			        FILE_ID, OSM_LOG_DEBUG);
	OSM_LOG(p_log, OSM_LOG_DEBUG, "InformInfo list size %d\n",
		cl_qlist_count(&p_subn->sa_infr_list));

	p_list_item = cl_qlist_find_from_head(&p_subn->sa_infr_list,
					      match_inf_rec, p_infr_rec);

	if (p_list_item == cl_qlist_end(&p_subn->sa_infr_list))
		p_list_item = NULL;

	OSM_LOG_EXIT(p_log);
	return (osm_infr_t *) p_list_item;
}