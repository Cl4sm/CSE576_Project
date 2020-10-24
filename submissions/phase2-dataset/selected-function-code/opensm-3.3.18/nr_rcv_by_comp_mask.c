static void nr_rcv_by_comp_mask(IN cl_map_item_t * p_map_item, IN void *context)
{
	const osm_nr_search_ctxt_t *p_ctxt = context;
	osm_node_t *p_node = (osm_node_t *) p_map_item;
	const ib_node_record_t *const p_rcvd_rec = p_ctxt->p_rcvd_rec;
	const osm_physp_t *const p_req_physp = p_ctxt->p_req_physp;
	osm_sa_t *sa = p_ctxt->sa;
	ib_net64_t comp_mask = p_ctxt->comp_mask;
	ib_net64_t match_port_guid = 0;
	ib_net16_t match_lid = 0;
	unsigned int match_port_num = 0;

	OSM_LOG_ENTER(p_ctxt->sa->p_log);

	osm_dump_node_info_v2(p_ctxt->sa->p_log, &p_node->node_info,
			      FILE_ID, OSM_LOG_DEBUG);

	if (comp_mask & IB_NR_COMPMASK_LID)
		match_lid = p_rcvd_rec->lid;

	if (comp_mask & IB_NR_COMPMASK_NODEGUID) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Looking for node 0x%016" PRIx64
			", found 0x%016" PRIx64 "\n",
			cl_ntoh64(p_rcvd_rec->node_info.node_guid),
			cl_ntoh64(osm_node_get_node_guid(p_node)));

		if (p_node->node_info.node_guid !=
		    p_rcvd_rec->node_info.node_guid)
			goto Exit;
	}

	if (comp_mask & IB_NR_COMPMASK_PORTGUID)
		match_port_guid = p_rcvd_rec->node_info.port_guid;

	if ((comp_mask & IB_NR_COMPMASK_SYSIMAGEGUID) &&
	    p_node->node_info.sys_guid != p_rcvd_rec->node_info.sys_guid)
			goto Exit;

	if ((comp_mask & IB_NR_COMPMASK_BASEVERSION) &&
	    p_node->node_info.base_version !=
	    p_rcvd_rec->node_info.base_version)
			goto Exit;

	if ((comp_mask & IB_NR_COMPMASK_CLASSVERSION) &&
	    p_node->node_info.class_version !=
	    p_rcvd_rec->node_info.class_version)
		goto Exit;

	if ((comp_mask & IB_NR_COMPMASK_NODETYPE) &&
	    p_node->node_info.node_type != p_rcvd_rec->node_info.node_type)
		goto Exit;

	if ((comp_mask & IB_NR_COMPMASK_NUMPORTS) &&
	    p_node->node_info.num_ports != p_rcvd_rec->node_info.num_ports)
		goto Exit;

	if ((comp_mask & IB_NR_COMPMASK_PARTCAP) &&
	    p_node->node_info.partition_cap !=
	    p_rcvd_rec->node_info.partition_cap)
		goto Exit;

	if ((comp_mask & IB_NR_COMPMASK_DEVID) &&
	    p_node->node_info.device_id != p_rcvd_rec->node_info.device_id)
		goto Exit;

	if ((comp_mask & IB_NR_COMPMASK_REV) &&
	    p_node->node_info.revision !=
	    p_rcvd_rec->node_info.revision)
		goto Exit;

	if (comp_mask & IB_NR_COMPMASK_PORTNUM)
		match_port_num = ib_node_info_get_local_port_num(&p_rcvd_rec->node_info);

	if ((comp_mask & IB_NR_COMPMASK_VENDID) &&
	    ib_node_info_get_vendor_id(&p_node->node_info) !=
	    ib_node_info_get_vendor_id(&p_rcvd_rec->node_info))
		goto Exit;

	if ((comp_mask & IB_NR_COMPMASK_NODEDESC) &&
	    strncmp((char *)&p_node->node_desc, (char *)&p_rcvd_rec->node_desc,
		    sizeof(ib_node_desc_t)))
		goto Exit;

	nr_rcv_create_nr(sa, p_node, p_ctxt->p_list, match_port_guid,
			 match_lid, match_port_num, p_req_physp, comp_mask);

Exit:
	OSM_LOG_EXIT(p_ctxt->sa->p_log);
}