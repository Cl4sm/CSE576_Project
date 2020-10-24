static void dump_ucast_lfts(cl_map_item_t * item, FILE * file, void *cxt)
{
	osm_switch_t *p_sw = (osm_switch_t *) item;
	osm_opensm_t *p_osm = cxt;
	osm_node_t *p_node = p_sw->p_node;
	unsigned max_lid = p_sw->max_lid_ho;
	unsigned max_port = p_sw->num_ports;
	uint16_t lid;
	uint8_t port;

	fprintf(file, "Unicast lids [0-%u] of switch Lid %u guid 0x%016"
		PRIx64 " (\'%s\'):\n",
		max_lid, cl_ntoh16(osm_node_get_base_lid(p_node, 0)),
		cl_ntoh64(osm_node_get_node_guid(p_node)), p_node->print_desc);
	for (lid = 0; lid <= max_lid; lid++) {
		osm_port_t *p_port;
		port = osm_switch_get_port_by_lid(p_sw, lid);

		if (port >= max_port)
			continue;

		fprintf(file, "0x%04x %03u # ", lid, port);

		p_port = osm_get_port_by_lid_ho(&p_osm->subn, lid);
		if (p_port) {
			p_node = p_port->p_node;
			fprintf(file, "%s portguid 0x%016" PRIx64 ": \'%s\'",
				ib_get_node_type_str(osm_node_get_type(p_node)),
				cl_ntoh64(osm_port_get_guid(p_port)),
				p_node->print_desc);
		} else
			fprintf(file, "unknown node and type");
		fprintf(file, "\n");
	}
	fprintf(file, "%u lids dumped\n", max_lid);
}