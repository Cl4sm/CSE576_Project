static void dump_topology_node(cl_map_item_t * item, FILE * file, void *cxt)
{
	osm_node_t *p_node = (osm_node_t *) item;
	uint32_t cPort;
	osm_node_t *p_nbnode;
	osm_physp_t *p_physp, *p_default_physp, *p_rphysp;
	uint8_t link_speed_act;
	const char *link_speed_act_str;

	if (!p_node->node_info.num_ports)
		return;

	for (cPort = 1; cPort < osm_node_get_num_physp(p_node); cPort++) {
		uint8_t port_state;

		p_physp = osm_node_get_physp_ptr(p_node, cPort);
		if (!p_physp)
			continue;

		p_rphysp = p_physp->p_remote_physp;
		if (!p_rphysp)
			continue;

		CL_ASSERT(cPort == p_physp->port_num);

		if (p_node->node_info.node_type == IB_NODE_TYPE_SWITCH)
			p_default_physp = osm_node_get_physp_ptr(p_node, 0);
		else
			p_default_physp = p_physp;

		fprintf(file, "{ %s%s Ports:%02X SystemGUID:%016" PRIx64
			" NodeGUID:%016" PRIx64 " PortGUID:%016" PRIx64
			" VenID:%06X DevID:%04X Rev:%08X {%s} LID:%04X PN:%02X } ",
			p_node->node_info.node_type == IB_NODE_TYPE_SWITCH ?
			"SW" : p_node->node_info.node_type ==
			IB_NODE_TYPE_CA ? "CA" : p_node->node_info.node_type ==
			IB_NODE_TYPE_ROUTER ? "Rt" : "**",
			p_default_physp->port_info.base_lid ==
			p_default_physp->port_info.
			master_sm_base_lid ? "-SM" : "",
			p_node->node_info.num_ports,
			cl_ntoh64(p_node->node_info.sys_guid),
			cl_ntoh64(p_node->node_info.node_guid),
			cl_ntoh64(p_physp->port_guid),
			cl_ntoh32(ib_node_info_get_vendor_id
				  (&p_node->node_info)),
			cl_ntoh16(p_node->node_info.device_id),
			cl_ntoh32(p_node->node_info.revision),
			p_node->print_desc,
			cl_ntoh16(p_default_physp->port_info.base_lid), cPort);

		p_nbnode = p_rphysp->p_node;

		if (p_nbnode->node_info.node_type == IB_NODE_TYPE_SWITCH)
			p_default_physp = osm_node_get_physp_ptr(p_nbnode, 0);
		else
			p_default_physp = p_rphysp;

		fprintf(file, "{ %s%s Ports:%02X SystemGUID:%016" PRIx64
			" NodeGUID:%016" PRIx64 " PortGUID:%016" PRIx64
			" VenID:%08X DevID:%04X Rev:%08X {%s} LID:%04X PN:%02X } ",
			p_nbnode->node_info.node_type == IB_NODE_TYPE_SWITCH ?
			"SW" : p_nbnode->node_info.node_type ==
			IB_NODE_TYPE_CA ? "CA" :
			p_nbnode->node_info.node_type == IB_NODE_TYPE_ROUTER ?
			"Rt" : "**",
			p_default_physp->port_info.base_lid ==
			p_default_physp->port_info.
			master_sm_base_lid ? "-SM" : "",
			p_nbnode->node_info.num_ports,
			cl_ntoh64(p_nbnode->node_info.sys_guid),
			cl_ntoh64(p_nbnode->node_info.node_guid),
			cl_ntoh64(p_rphysp->port_guid),
			cl_ntoh32(ib_node_info_get_vendor_id
				  (&p_nbnode->node_info)),
			cl_ntoh32(p_nbnode->node_info.device_id),
			cl_ntoh32(p_nbnode->node_info.revision),
			p_nbnode->print_desc,
			cl_ntoh16(p_default_physp->port_info.base_lid),
			p_rphysp->port_num);

		port_state = ib_port_info_get_port_state(&p_physp->port_info);
		link_speed_act =
		    ib_port_info_get_link_speed_active(&p_physp->port_info);
		if (link_speed_act == IB_LINK_SPEED_ACTIVE_2_5)
			link_speed_act_str = "2.5";
		else if (link_speed_act == IB_LINK_SPEED_ACTIVE_5)
			link_speed_act_str = "5";
		else if (link_speed_act == IB_LINK_SPEED_ACTIVE_10)
			link_speed_act_str = "10";
		else
			link_speed_act_str = "??";

		if (p_physp->ext_port_info.link_speed_active & FDR10)
			link_speed_act_str = "FDR10";

		if (p_default_physp->port_info.capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS) {
			link_speed_act =
			    ib_port_info_get_link_speed_ext_active(&p_physp->port_info);
			if (link_speed_act == IB_LINK_SPEED_EXT_ACTIVE_14)
				link_speed_act_str = "14";
			else if (link_speed_act == IB_LINK_SPEED_EXT_ACTIVE_25)
				link_speed_act_str = "25";
			else if (link_speed_act != IB_LINK_SPEED_EXT_ACTIVE_NONE)
				link_speed_act_str = "??";
		}

		fprintf(file, "PHY=%s LOG=%s SPD=%s\n",
			p_physp->port_info.link_width_active == 1 ? "1x" :
			p_physp->port_info.link_width_active == 2 ? "4x" :
			p_physp->port_info.link_width_active == 8 ? "12x" :
			"??",
			port_state == IB_LINK_ACTIVE ? "ACT" :
			port_state == IB_LINK_ARMED ? "ARM" :
			port_state == IB_LINK_INIT ? "INI" : "DWN",
			link_speed_act_str);
	}
}