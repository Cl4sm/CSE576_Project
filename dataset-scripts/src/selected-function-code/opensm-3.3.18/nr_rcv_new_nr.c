static ib_api_status_t nr_rcv_new_nr(osm_sa_t * sa,
				     IN const osm_node_t * p_node,
				     IN cl_qlist_t * p_list,
				     IN ib_net64_t port_guid, IN ib_net16_t lid,
	                             IN unsigned int port_num)
{
	osm_sa_item_t *p_rec_item;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_NR_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1D02: "
			"rec_item alloc failed\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"New NodeRecord: node 0x%016" PRIx64
		", port 0x%016" PRIx64 ", lid %u\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)),
		cl_ntoh64(port_guid), cl_ntoh16(lid));

	memset(p_rec_item, 0, SA_NR_RESP_SIZE);

	p_rec_item->resp.node_rec.lid = lid;

	p_rec_item->resp.node_rec.node_info = p_node->node_info;
	p_rec_item->resp.node_rec.node_info.port_guid = port_guid;
	p_rec_item->resp.node_rec.node_info.port_num_vendor_id =
		(p_rec_item->resp.node_rec.node_info.port_num_vendor_id & IB_NODE_INFO_VEND_ID_MASK) |
		((port_num << IB_NODE_INFO_PORT_NUM_SHIFT) & IB_NODE_INFO_PORT_NUM_MASK);
	memcpy(&(p_rec_item->resp.node_rec.node_desc), &(p_node->node_desc),
	       IB_NODE_DESCRIPTION_SIZE);
	cl_qlist_insert_tail(p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}