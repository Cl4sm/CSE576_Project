static void infr_rcv_respond(IN osm_sa_t * sa, IN osm_madw_t * p_madw)
{
	cl_qlist_t rec_list;
	osm_sa_item_t *item;

	OSM_LOG_ENTER(sa->p_log);

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Generating successful InformInfo response\n");

	item = malloc(SA_II_RESP_SIZE);
	if (!item) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4303: "
			"rec_item alloc failed\n");
		goto Exit;
	}

	memcpy(&item->resp.inform,
	       ib_sa_mad_get_payload_ptr(osm_madw_get_sa_mad_ptr(p_madw)),
	       sizeof(ib_inform_info_t));

	cl_qlist_init(&rec_list);
	cl_qlist_insert_tail(&rec_list, &item->list_item);

	osm_sa_respond(sa, p_madw, sizeof(ib_inform_info_t), &rec_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}