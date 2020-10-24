static void dump_ucast_path_distribution(cl_map_item_t * item, FILE * file,
					 void *cxt)
{
	osm_node_t *p_node;
	osm_node_t *p_remote_node;
	uint8_t i;
	uint8_t num_ports;
	uint32_t num_paths;
	ib_net64_t remote_guid_ho;
	osm_switch_t *p_sw = (osm_switch_t *) item;

	p_node = p_sw->p_node;
	num_ports = p_sw->num_ports;

	fprintf(file, "dump_ucast_path_distribution: Switch 0x%" PRIx64 "\n"
		"Port : Path Count Through Port",
		cl_ntoh64(osm_node_get_node_guid(p_node)));

	for (i = 0; i < num_ports; i++) {
		num_paths = osm_switch_path_count_get(p_sw, i);
		fprintf(file, "\n %03u : %u", i, num_paths);
		if (i == 0) {
			fprintf(file, " (switch management port)");
			continue;
		}

		p_remote_node = osm_node_get_remote_node(p_node, i, NULL);
		if (p_remote_node == NULL)
			continue;

		remote_guid_ho =
		    cl_ntoh64(osm_node_get_node_guid(p_remote_node));

		switch (osm_node_get_type(p_remote_node)) {
		case IB_NODE_TYPE_SWITCH:
			fprintf(file, " (link to switch");
			break;
		case IB_NODE_TYPE_ROUTER:
			fprintf(file, " (link to router");
			break;
		case IB_NODE_TYPE_CA:
			fprintf(file, " (link to CA");
			break;
		default:
			fprintf(file, " (link to unknown node type");
			break;
		}

		fprintf(file, " 0x%" PRIx64 ")", remote_guid_ho);
	}

	fprintf(file, "\n");
}