static void lidbalance_check(osm_opensm_t * p_osm,
			     osm_switch_t * p_sw, FILE * out)
{
	uint8_t port_num;
	const cl_qmap_t *p_port_tbl;
	osm_port_t *p_port;

	p_port_tbl = &p_osm->subn.port_guid_tbl;
	for (p_port = (osm_port_t *) cl_qmap_head(p_port_tbl);
	     p_port != (osm_port_t *) cl_qmap_end(p_port_tbl);
	     p_port = (osm_port_t *) cl_qmap_next(&p_port->map_item)) {
		uint32_t port_count[255];	/* max ports is a uint8_t */
		osm_node_t *rem_node[255];
		uint32_t rem_node_count;
		uint32_t rem_count[255];
		osm_physp_t *p_physp;
		osm_physp_t *p_rem_physp;
		osm_node_t *p_rem_node;
		uint32_t port_min_count = 0xFFFFFFFF;
		uint32_t port_max_count = 0;
		uint32_t rem_min_count = 0xFFFFFFFF;
		uint32_t rem_max_count = 0;
		uint16_t min_lid_ho;
		uint16_t max_lid_ho;
		uint16_t lid_ho;
		uint8_t num_ports;
		unsigned int i;

		/* we only care about non-switches */
		if (osm_node_get_type(p_port->p_node) == IB_NODE_TYPE_SWITCH)
			continue;

		osm_port_get_lid_range_ho(p_port, &min_lid_ho, &max_lid_ho);

		if (min_lid_ho == 0 || max_lid_ho == 0)
			continue;

		memset(port_count, '\0', sizeof(uint32_t) * 255);
		memset(rem_node, '\0', sizeof(osm_node_t *) * 255);
		rem_node_count = 0;
		memset(rem_count, '\0', sizeof(uint32_t) * 255);

		for (lid_ho = min_lid_ho; lid_ho <= max_lid_ho; lid_ho++) {
			boolean_t rem_node_found = FALSE;
			unsigned int indx = 0;

			port_num = osm_switch_get_port_by_lid(p_sw, lid_ho);
			if (port_num == OSM_NO_PATH)
				continue;

			p_physp =
			    osm_node_get_physp_ptr(p_sw->p_node, port_num);

			/* if port is down/unhealthy, can't calculate */
			if (!p_physp || !osm_physp_is_healthy(p_physp)
			    || !osm_physp_get_remote(p_physp))
				continue;

			p_rem_physp = osm_physp_get_remote(p_physp);
			p_rem_node = osm_physp_get_node_ptr(p_rem_physp);

			/* determine if we've seen this remote node before.
			 * If not, store it.  If yes, update the counter
			 */
			for (i = 0; i < rem_node_count; i++) {
				if (rem_node[i] == p_rem_node) {
					rem_node_found = TRUE;
					indx = i;
					break;
				}
			}

			if (!rem_node_found) {
				rem_node[rem_node_count] = p_rem_node;
				rem_count[rem_node_count]++;
				indx = rem_node_count;
				rem_node_count++;
			} else
				rem_count[indx]++;

			port_count[port_num]++;
		}

		if (!rem_node_count)
			continue;

		for (i = 0; i < rem_node_count; i++) {
			if (rem_count[i] < rem_min_count)
				rem_min_count = rem_count[i];
			if (rem_count[i] > rem_max_count)
				rem_max_count = rem_count[i];
		}

		num_ports = p_sw->num_ports;
		for (i = 0; i < num_ports; i++) {
			if (!port_count[i])
				continue;
			if (port_count[i] < port_min_count)
				port_min_count = port_count[i];
			if (port_count[i] > port_max_count)
				port_max_count = port_count[i];
		}

		/* Output if this CA/router is being forwarded an unbalanced number of
		 * times to a destination.
		 */
		if ((rem_max_count - rem_min_count) > 1) {
			fprintf(out,
				"Unbalanced Remote Forwarding: Switch 0x%016"
				PRIx64 " (%s): ",
				cl_ntoh64(p_sw->p_node->node_info.node_guid),
				p_sw->p_node->print_desc);
			if (osm_node_get_type(p_port->p_node) ==
			    IB_NODE_TYPE_CA)
				fprintf(out, "CA");
			else if (osm_node_get_type(p_port->p_node) ==
				 IB_NODE_TYPE_ROUTER)
				fprintf(out, "Router");
			fprintf(out, " 0x%016" PRIx64 " (%s): ",
				cl_ntoh64(p_port->p_node->node_info.node_guid),
				p_port->p_node->print_desc);
			for (i = 0; i < rem_node_count; i++) {
				fprintf(out,
					"Dest 0x%016" PRIx64 "(%s) - %u ",
					cl_ntoh64(rem_node[i]->node_info.
						  node_guid),
					rem_node[i]->print_desc, rem_count[i]);
			}
			fprintf(out, "\n");
		}

		/* Output if this CA/router is being forwarded through a port
		 * an unbalanced number of times.
		 */
		if ((port_max_count - port_min_count) > 1) {
			fprintf(out,
				"Unbalanced Port Forwarding: Switch 0x%016"
				PRIx64 " (%s): ",
				cl_ntoh64(p_sw->p_node->node_info.node_guid),
				p_sw->p_node->print_desc);
			if (osm_node_get_type(p_port->p_node) ==
			    IB_NODE_TYPE_CA)
				fprintf(out, "CA");
			else if (osm_node_get_type(p_port->p_node) ==
				 IB_NODE_TYPE_ROUTER)
				fprintf(out, "Router");
			fprintf(out, " 0x%016" PRIx64 " (%s): ",
				cl_ntoh64(p_port->p_node->node_info.node_guid),
				p_port->p_node->print_desc);
			for (i = 0; i < num_ports; i++) {
				if (!port_count[i])
					continue;
				fprintf(out, "Port %u - %u: ", i,
					port_count[i]);
			}
			fprintf(out, "\n");
		}
	}
}