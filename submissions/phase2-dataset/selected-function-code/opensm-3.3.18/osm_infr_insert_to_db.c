void osm_infr_insert_to_db(IN osm_subn_t * p_subn, IN osm_log_t * p_log,
			   IN osm_infr_t * p_infr)
{
	OSM_LOG_ENTER(p_log);

	OSM_LOG(p_log, OSM_LOG_DEBUG,
		"Inserting new InformInfo Record into Database\n");
	OSM_LOG(p_log, OSM_LOG_DEBUG, "Dump before insertion (size %d)\n",
		cl_qlist_count(&p_subn->sa_infr_list));
	dump_all_informs(p_subn, p_log);

#if 0
	osm_dump_inform_info_v2(p_log,
			        &(p_infr->inform_record.inform_info),
			        FILE_ID, OSM_LOG_DEBUG);
#endif

	cl_qlist_insert_head(&p_subn->sa_infr_list, &p_infr->list_item);
	p_subn->p_osm->sa.dirty = TRUE;

	OSM_LOG(p_log, OSM_LOG_DEBUG, "Dump after insertion (size %d)\n",
		cl_qlist_count(&p_subn->sa_infr_list));
	dump_all_informs(p_subn, p_log);
	OSM_LOG_EXIT(p_log);
}