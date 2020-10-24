static void guidinfo_respond(IN osm_sa_t *sa, IN osm_madw_t *p_madw,
			     IN ib_guidinfo_record_t * p_guidinfo_rec)
{
	cl_qlist_t rec_list;
	osm_sa_item_t *item;

	OSM_LOG_ENTER(sa->p_log);

	item = malloc(SA_GIR_RESP_SIZE);
	if (!item) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5101: "
			"rec_item alloc failed\n");
		goto Exit;
	}

	item->resp.guid_rec = *p_guidinfo_rec;

	cl_qlist_init(&rec_list);
	cl_qlist_insert_tail(&rec_list, &item->list_item);

	osm_sa_respond(sa, p_madw, sizeof(ib_guidinfo_record_t), &rec_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}