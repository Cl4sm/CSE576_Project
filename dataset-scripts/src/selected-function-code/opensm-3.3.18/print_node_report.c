static void print_node_report(cl_map_item_t * item, FILE * file, void *cxt)
{
	osm_node_t *p_node = (osm_node_t *) item;
	osm_opensm_t *osm = cxt;
	const osm_physp_t *p_physp, *p_remote_physp;
	const ib_port_info_t *p_pi;
	uint8_t port_num;
	uint32_t num_ports;
	uint8_t node_type;

	node_type = osm_node_get_type(p_node);

	num_ports = osm_node_get_num_physp(p_node);
	port_num = node_type == IB_NODE_TYPE_SWITCH ? 0 : 1;
	for (; port_num < num_ports; port_num++) {
		p_physp = osm_node_get_physp_ptr(p_node, port_num);
		if (!p_physp)
			continue;

		fprintf(file, "%-11s : %s : %02X :",
			osm_get_manufacturer_str(cl_ntoh64
						 (osm_node_get_node_guid
						  (p_node))),
			osm_get_node_type_str_fixed_width(node_type), port_num);

		p_pi = &p_physp->port_info;

		/*
		 * Port state is not defined for base switch port 0
		 */
		if (port_num == 0 &&
		    ib_switch_info_is_enhanced_port0(&p_node->sw->switch_info) == FALSE)
			fprintf(file, "     :");
		else
			fprintf(file, " %s :",
				osm_get_port_state_str_fixed_width
				(ib_port_info_get_port_state(p_pi)));

		/*
		 * LID values are only meaningful in select cases.
		 */
		if (ib_port_info_get_port_state(p_pi) != IB_LINK_DOWN
		    && ((node_type == IB_NODE_TYPE_SWITCH && port_num == 0)
			|| node_type != IB_NODE_TYPE_SWITCH))
			fprintf(file, " %04X :  %01X  :",
				cl_ntoh16(p_pi->base_lid),
				ib_port_info_get_lmc(p_pi));
		else
			fprintf(file, "      :     :");

		if (port_num == 0 &&
		    ib_switch_info_is_enhanced_port0(&p_node->sw->switch_info) == FALSE)
			fprintf(file, "      :     :      ");
		else
			fprintf(file, " %s : %s : %s ",
				osm_get_mtu_str
				(ib_port_info_get_neighbor_mtu(p_pi)),
				osm_get_lwa_str(p_pi->link_width_active),
				osm_get_lsa_str
				(ib_port_info_get_link_speed_active(p_pi),
				 ib_port_info_get_link_speed_ext_active(p_pi),
				 ib_port_info_get_port_state(p_pi),
				 p_physp->ext_port_info.link_speed_active & FDR10));

		if (osm_physp_get_port_guid(p_physp) == osm->subn.sm_port_guid)
			fprintf(file, "* %016" PRIx64 " *",
				cl_ntoh64(osm_physp_get_port_guid(p_physp)));
		else
			fprintf(file, ": %016" PRIx64 " :",
				cl_ntoh64(osm_physp_get_port_guid(p_physp)));

		if (port_num
		    && (ib_port_info_get_port_state(p_pi) != IB_LINK_DOWN)) {
			p_remote_physp = osm_physp_get_remote(p_physp);
			if (p_remote_physp)
				fprintf(file, " %016" PRIx64 " (%02X)",
					cl_ntoh64(osm_physp_get_port_guid
						  (p_remote_physp)),
					osm_physp_get_port_num(p_remote_physp));
			else
				fprintf(file, " UNKNOWN");
		}

		fprintf(file, "\n");
	}

	fprintf(file, "------------------------------------------------------"
		"------------------------------------------------\n");
}