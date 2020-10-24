static void lr_rcv_build_physp_link(IN osm_sa_t * sa, IN ib_net16_t from_lid,
				    IN ib_net16_t to_lid, IN uint8_t from_port,
				    IN uint8_t to_port, IN cl_qlist_t * p_list)
{
	osm_sa_item_t *p_lr_item;

	p_lr_item = malloc(SA_LR_RESP_SIZE);
	if (p_lr_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1801: "
			"Unable to acquire link record\n"
			"\t\t\t\tFrom port %u\n" "\t\t\t\tTo port   %u\n"
			"\t\t\t\tFrom lid  %u\n" "\t\t\t\tTo lid    %u\n",
			from_port, to_port,
			cl_ntoh16(from_lid), cl_ntoh16(to_lid));
		return;
	}
	memset(p_lr_item, 0, SA_LR_RESP_SIZE);

	p_lr_item->resp.link_rec.from_port_num = from_port;
	p_lr_item->resp.link_rec.to_port_num = to_port;
	p_lr_item->resp.link_rec.to_lid = to_lid;
	p_lr_item->resp.link_rec.from_lid = from_lid;

	cl_qlist_insert_tail(p_list, &p_lr_item->list_item);
}