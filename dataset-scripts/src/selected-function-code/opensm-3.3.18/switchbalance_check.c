static void switchbalance_check(osm_opensm_t * p_osm,
				osm_switch_t * p_sw, FILE * out, int verbose)
{
	uint8_t port_num;
	uint8_t num_ports;
	const cl_qmap_t *p_port_tbl;
	osm_port_t *p_port;
	osm_physp_t *p_physp;
	osm_physp_t *p_rem_physp;
	osm_node_t *p_rem_node;
	uint32_t count[255];	/* max ports is a uint8_t */
	uint8_t output_ports[255];
	uint8_t output_ports_count = 0;
	uint32_t min_count = 0xFFFFFFFF;
	uint32_t max_count = 0;
	unsigned int i;

	memset(count, '\0', sizeof(uint32_t) * 255);

	/* Count port usage */
	p_port_tbl = &p_osm->subn.port_guid_tbl;
	for (p_port = (osm_port_t *) cl_qmap_head(p_port_tbl);
	     p_port != (osm_port_t *) cl_qmap_end(p_port_tbl);
	     p_port = (osm_port_t *) cl_qmap_next(&p_port->map_item)) {
		uint16_t min_lid_ho;
		uint16_t max_lid_ho;
		uint16_t lid_ho;

		/* Don't count switches in port usage */
		if (osm_node_get_type(p_port->p_node) == IB_NODE_TYPE_SWITCH)
			continue;

		osm_port_get_lid_range_ho(p_port, &min_lid_ho, &max_lid_ho);

		if (min_lid_ho == 0 || max_lid_ho == 0)
			continue;

		for (lid_ho = min_lid_ho; lid_ho <= max_lid_ho; lid_ho++) {
			port_num = osm_switch_get_port_by_lid(p_sw, lid_ho);
			if (port_num == OSM_NO_PATH)
				continue;

			count[port_num]++;
		}
	}

	num_ports = p_sw->num_ports;
	for (port_num = 1; port_num < num_ports; port_num++) {
		p_physp = osm_node_get_physp_ptr(p_sw->p_node, port_num);

		/* if port is down/unhealthy, don't consider it in
		 * min/max calculations
		 */
		if (!p_physp || !osm_physp_is_healthy(p_physp)
		    || !osm_physp_get_remote(p_physp))
			continue;

		p_rem_physp = osm_physp_get_remote(p_physp);
		p_rem_node = osm_physp_get_node_ptr(p_rem_physp);

		/* If we are directly connected to a CA/router, its not really
		 * up for balancing consideration.
		 */
		if (osm_node_get_type(p_rem_node) != IB_NODE_TYPE_SWITCH)
			continue;

		output_ports[output_ports_count] = port_num;
		output_ports_count++;

		if (count[port_num] < min_count)
			min_count = count[port_num];
		if (count[port_num] > max_count)
			max_count = count[port_num];
	}

	if (verbose || ((max_count - min_count) > 1)) {
		if ((max_count - min_count) > 1)
			fprintf(out,
				"Unbalanced Switch: 0x%016" PRIx64 " (%s)\n",
				cl_ntoh64(p_sw->p_node->node_info.node_guid),
				p_sw->p_node->print_desc);
		else
			fprintf(out,
				"Switch: 0x%016" PRIx64 " (%s)\n",
				cl_ntoh64(p_sw->p_node->node_info.node_guid),
				p_sw->p_node->print_desc);

		for (i = 0; i < output_ports_count; i++) {
			fprintf(out,
				"Port %d: %d\n",
				output_ports[i], count[output_ports[i]]);
		}
	}
}