static void ni_rcv_process_new(IN osm_sm_t * sm, IN const osm_madw_t * p_madw)
{
	osm_node_t *p_node;
	osm_node_t *p_node_check;
	osm_port_t *p_port;
	osm_port_t *p_port_check;
	osm_router_t *p_rtr = NULL;
	osm_router_t *p_rtr_check;
	cl_qmap_t *p_rtr_guid_tbl;
	ib_node_info_t *p_ni;
	ib_smp_t *p_smp;
	osm_ni_context_t *p_ni_context;
	osm_alias_guid_t *p_alias_guid, *p_alias_guid_check;
	uint8_t port_num;

	OSM_LOG_ENTER(sm->p_log);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_ni = ib_smp_get_payload_ptr(p_smp);
	p_ni_context = osm_madw_get_ni_context_ptr(p_madw);
	port_num = ib_node_info_get_local_port_num(p_ni);

	osm_dump_smp_dr_path_v2(sm->p_log, p_smp, FILE_ID, OSM_LOG_VERBOSE);

	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
		"Discovered new %s node,"
		"\n\t\t\t\tGUID 0x%" PRIx64 ", TID 0x%" PRIx64 "\n",
		ib_get_node_type_str(p_ni->node_type),
		cl_ntoh64(p_ni->node_guid), cl_ntoh64(p_smp->trans_id));

	if (PF(port_num > p_ni->num_ports)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D0A: "
			"New %s node GUID 0x%" PRIx64 "is non-compliant and "
			"is being ignored since the "
			"local port num %u > num ports %u\n",
			ib_get_node_type_str(p_ni->node_type),
			cl_ntoh64(p_ni->node_guid), port_num,
			p_ni->num_ports);
		goto Exit;
	}

	p_node = osm_node_new(p_madw);
	if (PF(p_node == NULL)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D07: "
			"Unable to create new node object\n");
		goto Exit;
	}

	/*
	   Create a new port object to represent this node's physical
	   ports in the port table.
	 */
	p_port = osm_port_new(p_ni, p_node);
	if (PF(p_port == NULL)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D14: "
			"Unable to create new port object\n");
		osm_node_delete(&p_node);
		goto Exit;
	}

	/*
	   Add the new port object to the database.
	 */
	p_port_check =
	    (osm_port_t *) cl_qmap_insert(&sm->p_subn->port_guid_tbl,
					  p_ni->port_guid, &p_port->map_item);
	if (PF(p_port_check != p_port)) {
		/*
		   We should never be here!
		   Somehow, this port GUID already exists in the table.
		 */
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D15: "
			"Duplicate Port GUID 0x%" PRIx64
			"! Found by the two directed routes:\n",
			cl_ntoh64(p_ni->port_guid));
		osm_dump_dr_path_v2(sm->p_log,
				    osm_physp_get_dr_path_ptr(p_port->p_physp),
				    FILE_ID, OSM_LOG_ERROR);
		osm_dump_dr_path_v2(sm->p_log,
				    osm_physp_get_dr_path_ptr(p_port_check->
							   p_physp),
				    FILE_ID, OSM_LOG_ERROR);
		osm_port_delete(&p_port);
		osm_node_delete(&p_node);
		goto Exit;
	}

	p_alias_guid = osm_alias_guid_new(p_ni->port_guid,
					  p_port);
	if (PF(!p_alias_guid)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D18: "
			"alias guid memory allocation failed"
			" for port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(p_ni->port_guid));
		goto alias_done2;
	}

	/* insert into alias guid table */
	p_alias_guid_check =
		(osm_alias_guid_t *) cl_qmap_insert(&sm->p_subn->alias_port_guid_tbl,
						    p_alias_guid->alias_guid,
						    &p_alias_guid->map_item);
	if (p_alias_guid_check != p_alias_guid) {
		/* alias GUID is a duplicate */
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D19: "
			"Duplicate alias port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(p_ni->port_guid));
		osm_alias_guid_delete(&p_alias_guid);
	}

alias_done2:
	/* If we are a master, then this means the port is new on the subnet.
	   Mark it as new - need to send trap 64 on these ports.
	   The condition that we are master is true, since if we are in discovering
	   state (meaning we woke up from standby or we are just initializing),
	   then these ports may be new to us, but are not new on the subnet.
	   If we are master, then the subnet as we know it is the updated one,
	   and any new ports we encounter should cause trap 64. C14-72.1.1 */
	if (sm->p_subn->sm_state == IB_SMINFO_STATE_MASTER)
		p_port->is_new = 1;

	/* If there were RouterInfo or other router attribute,
	   this would be elsewhere */
	if (p_ni->node_type == IB_NODE_TYPE_ROUTER) {
		if (PF((p_rtr = osm_router_new(p_port)) == NULL))
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D1A: "
				"Unable to create new router object\n");
		else {
			p_rtr_guid_tbl = &sm->p_subn->rtr_guid_tbl;
			p_rtr_check =
			    (osm_router_t *) cl_qmap_insert(p_rtr_guid_tbl,
							    p_ni->port_guid,
							    &p_rtr->map_item);
			if (PF(p_rtr_check != p_rtr))
				OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D1B: "
					"Unable to add port GUID:0x%016" PRIx64
					" to router table\n",
					cl_ntoh64(p_ni->port_guid));
		}
	}

	p_node_check =
	    (osm_node_t *) cl_qmap_insert(&sm->p_subn->node_guid_tbl,
					  p_ni->node_guid, &p_node->map_item);
	if (PF(p_node_check != p_node)) {
		/*
		   This node must have been inserted by another thread.
		   This is unexpected, but is not an error.
		   We can simply clean-up, since the other thread will
		   see this processing through to completion.
		 */
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Discovery race detected at node 0x%" PRIx64 "\n",
			cl_ntoh64(p_ni->node_guid));
		osm_node_delete(&p_node);
		p_node = p_node_check;
		ni_rcv_set_links(sm, p_node, port_num, p_ni_context);
		goto Exit;
	} else
		ni_rcv_set_links(sm, p_node, port_num, p_ni_context);

	p_node->discovery_count++;
	ni_rcv_get_node_desc(sm, p_node, p_madw);

	switch (p_ni->node_type) {
	case IB_NODE_TYPE_CA:
	case IB_NODE_TYPE_ROUTER:
		ni_rcv_process_new_ca_or_router(sm, p_node, p_madw);
		break;
	case IB_NODE_TYPE_SWITCH:
		ni_rcv_process_new_switch(sm, p_node, p_madw);
		break;
	default:
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D16: "
			"Unknown node type %u with GUID 0x%" PRIx64 "\n",
			p_ni->node_type, cl_ntoh64(p_ni->node_guid));
		break;
	}

Exit:
	OSM_LOG_EXIT(sm->p_log);
}