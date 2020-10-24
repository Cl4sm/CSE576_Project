static ib_api_status_t sir_rcv_new_sir(IN osm_sa_t * sa,
				       IN const osm_switch_t * p_sw,
				       IN cl_qlist_t * p_list,
				       IN ib_net16_t lid)
{
	osm_sa_item_t *p_rec_item;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_SIR_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5308: "
			"rec_item alloc failed\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"New SwitchInfoRecord: lid %u\n", cl_ntoh16(lid));

	memset(p_rec_item, 0, SA_SIR_RESP_SIZE);

	p_rec_item->resp.swinfo_rec.lid = lid;
	p_rec_item->resp.swinfo_rec.switch_info = p_sw->switch_info;

	cl_qlist_insert_tail(p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}