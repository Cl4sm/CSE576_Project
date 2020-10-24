static void __get_stats(cl_map_item_t * const p_map_item, void *context)
{
	fabric_stats_t *fs = (fabric_stats_t *) context;
	osm_node_t *node = (osm_node_t *) p_map_item;
	osm_physp_t *physp0;
	ib_port_info_t *pi0;
	uint8_t num_ports = osm_node_get_num_physp(node);
	uint8_t port = 0;

	/* Skip nodes we are not interested in */
	if (fs->node_type_lim != 0
	    && fs->node_type_lim != node->node_info.node_type)
		return;

	fs->total_nodes++;

	if (osm_node_get_type(node) == IB_NODE_TYPE_SWITCH) {
		physp0 = osm_node_get_physp_ptr(node, 0);
		pi0 = &physp0->port_info;
	} else
		pi0 = NULL;

	for (port = 1; port < num_ports; port++) {
		osm_physp_t *phys = osm_node_get_physp_ptr(node, port);
		ib_port_info_t *pi = NULL;
		ib_mlnx_ext_port_info_t *epi = NULL;
		uint8_t active_speed = 0;
		uint8_t enabled_speed = 0;
		uint8_t active_width = 0;
		uint8_t enabled_width = 0;
		uint8_t port_state = 0;
		uint8_t port_phys_state = 0;

		if (!phys)
			continue;

		pi = &phys->port_info;
		epi = &phys->ext_port_info;
		if (!pi0)
			pi0 = pi;
		active_speed = ib_port_info_get_link_speed_active(pi);
		enabled_speed = ib_port_info_get_link_speed_enabled(pi);
		active_width = pi->link_width_active;
		enabled_width = pi->link_width_enabled;
		port_state = ib_port_info_get_port_state(pi);
		port_phys_state = ib_port_info_get_port_phys_state(pi);

		if (port_state == IB_LINK_DOWN)
			fs->ports_down++;
		else if (port_state == IB_LINK_ACTIVE)
			fs->ports_active++;
		if (port_phys_state == IB_PORT_PHYS_STATE_DISABLED) {
			__tag_port_report(&(fs->disabled_ports),
					  cl_ntoh64(node->node_info.node_guid),
					  port, node->print_desc);
			fs->ports_disabled++;
		}

		fs->total_ports++;

		if (port_state == IB_LINK_DOWN)
			continue;

		if (!(active_width & enabled_width)) {
			__tag_port_report(&(fs->unenabled_width_ports),
					  cl_ntoh64(node->node_info.node_guid),
					  port, node->print_desc);
			fs->ports_unenabled_width++;
		}
		else if ((enabled_width ^ active_width) > active_width) {
			__tag_port_report(&(fs->reduced_width_ports),
					  cl_ntoh64(node->node_info.node_guid),
					  port, node->print_desc);
			fs->ports_reduced_width++;
		}

		/* unenabled speed usually due to problems with force_link_speed */
		if (!(active_speed & enabled_speed)) {
			__tag_port_report(&(fs->unenabled_speed_ports),
					  cl_ntoh64(node->node_info.node_guid),
					  port, node->print_desc);
			fs->ports_unenabled_speed++;
		}
		else if ((enabled_speed ^ active_speed) > active_speed) {
			__tag_port_report(&(fs->reduced_speed_ports),
					  cl_ntoh64(node->node_info.node_guid),
					  port, node->print_desc);
			fs->ports_reduced_speed++;
		}

		switch (active_speed) {
		case IB_LINK_SPEED_ACTIVE_2_5:
			fs->ports_sdr++;
			break;
		case IB_LINK_SPEED_ACTIVE_5:
			fs->ports_ddr++;
			break;
		case IB_LINK_SPEED_ACTIVE_10:
			if (!(pi0->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS) ||
			    ((pi0->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS) &&
			    !ib_port_info_get_link_speed_ext_active(pi))) {
				if (epi->link_speed_active & FDR10)
					fs->ports_fdr10++;
				else {
					fs->ports_qdr++;
					/* check for speed reduced from FDR10 */
					if (epi->link_speed_enabled & FDR10) {
						__tag_port_report(&(fs->reduced_speed_ports),
								  cl_ntoh64(node->node_info.node_guid),
								  port, node->print_desc);
						fs->ports_reduced_speed++;
					}
				}
			}
			break;
		default:
			fs->ports_unknown_speed++;
			break;
		}
		if (pi0->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS &&
		    ib_port_info_get_link_speed_ext_sup(pi) &&
		    (enabled_speed = ib_port_info_get_link_speed_ext_enabled(pi)) != IB_LINK_SPEED_EXT_DISABLE &&
		    active_speed == IB_LINK_SPEED_ACTIVE_10) {
			active_speed = ib_port_info_get_link_speed_ext_active(pi);
			if (!(active_speed & enabled_speed)) {
				__tag_port_report(&(fs->unenabled_speed_ports),
						  cl_ntoh64(node->node_info.node_guid),
						  port, node->print_desc);
				fs->ports_unenabled_speed++;
			}
			else if ((enabled_speed ^ active_speed) > active_speed) {
				__tag_port_report(&(fs->reduced_speed_ports),
						  cl_ntoh64(node->node_info.node_guid),
						  port, node->print_desc);
				fs->ports_reduced_speed++;
			}
			switch (active_speed) {
			case IB_LINK_SPEED_EXT_ACTIVE_14:
				fs->ports_fdr++;
				break;
			case IB_LINK_SPEED_EXT_ACTIVE_25:
				fs->ports_edr++;
				break;
			default:
				fs->ports_unknown_speed++;
				break;
			}
		}
		switch (active_width) {
		case IB_LINK_WIDTH_ACTIVE_1X:
			fs->ports_1X++;
			break;
		case IB_LINK_WIDTH_ACTIVE_4X:
			fs->ports_4X++;
			break;
		case IB_LINK_WIDTH_ACTIVE_8X:
			fs->ports_8X++;
			break;
		case IB_LINK_WIDTH_ACTIVE_12X:
			fs->ports_12X++;
			break;
		default:
			fs->ports_unknown_width++;
			break;
		}
	}
}