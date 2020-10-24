static void mcmr_rcv_respond(IN osm_sa_t * sa, IN osm_madw_t * p_madw,
			     IN ib_member_rec_t * p_mcmember_rec)
{
	cl_qlist_t rec_list;
	osm_sa_item_t *item;

	OSM_LOG_ENTER(sa->p_log);

	item = malloc(SA_MCM_RESP_SIZE);
	if (!item) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B16: "
			"rec_item alloc failed\n");
		goto Exit;
	}

	item->resp.mc_rec = *p_mcmember_rec;

	/* Fill in the mtu, rate, and packet lifetime selectors */
	item->resp.mc_rec.mtu &= 0x3f;
	item->resp.mc_rec.mtu |= IB_PATH_SELECTOR_EXACTLY << 6;
	item->resp.mc_rec.rate &= 0x3f;
	item->resp.mc_rec.rate |= IB_PATH_SELECTOR_EXACTLY << 6;
	item->resp.mc_rec.pkt_life &= 0x3f;
	item->resp.mc_rec.pkt_life |= IB_PATH_SELECTOR_EXACTLY << 6;

	cl_qlist_init(&rec_list);
	cl_qlist_insert_tail(&rec_list, &item->list_item);

	osm_sa_respond(sa, p_madw, sizeof(ib_member_rec_t), &rec_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}