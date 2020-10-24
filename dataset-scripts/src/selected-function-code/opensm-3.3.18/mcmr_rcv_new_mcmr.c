static ib_api_status_t mcmr_rcv_new_mcmr(IN osm_sa_t * sa,
					 IN const ib_member_rec_t * p_rcvd_rec,
					 IN cl_qlist_t * p_list)
{
	osm_sa_item_t *p_rec_item;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_MCM_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B15: "
			"rec_item alloc failed\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	memset(p_rec_item, 0, sizeof(cl_list_item_t));

	/* HACK: Untrusted requesters should result with 0 Join
	   State, Port Guid, and Proxy */
	p_rec_item->resp.mc_rec = *p_rcvd_rec;
	cl_qlist_insert_tail(p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}