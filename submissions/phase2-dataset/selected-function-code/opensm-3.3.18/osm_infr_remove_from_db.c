void osm_infr_remove_from_db(IN osm_subn_t * p_subn, IN osm_log_t * p_log,
			     IN osm_infr_t * p_infr)
{
	char gid_str[INET6_ADDRSTRLEN];
	OSM_LOG_ENTER(p_log);

	OSM_LOG(p_log, OSM_LOG_DEBUG, "Removing InformInfo Subscribing GID:%s"
		" Enum:0x%X from Database\n",
		inet_ntop(AF_INET6, p_infr->inform_record.subscriber_gid.raw,
			  gid_str, sizeof gid_str),
		p_infr->inform_record.subscriber_enum);

	osm_dump_inform_info_v2(p_log, &(p_infr->inform_record.inform_info),
			        FILE_ID, OSM_LOG_DEBUG);

	cl_qlist_remove_item(&p_subn->sa_infr_list, &p_infr->list_item);
	p_subn->p_osm->sa.dirty = TRUE;

	osm_infr_delete(p_infr);

	OSM_LOG_EXIT(p_log);
}