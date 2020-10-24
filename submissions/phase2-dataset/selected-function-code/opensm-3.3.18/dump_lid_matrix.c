static void dump_lid_matrix(cl_map_item_t * item, FILE * file, void *cxt)
{
	osm_switch_t *p_sw = (osm_switch_t *) item;
	osm_opensm_t *p_osm = cxt;
	osm_node_t *p_node = p_sw->p_node;
	unsigned max_lid = p_sw->max_lid_ho;
	unsigned max_port = p_sw->num_ports;
	uint16_t lid;
	uint8_t port;

	fprintf(file, "Switch: guid 0x%016" PRIx64 "\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)));
	for (lid = 1; lid <= max_lid; lid++) {
		osm_port_t *p_port;
		if (osm_switch_get_least_hops(p_sw, lid) == OSM_NO_PATH)
			continue;
		fprintf(file, "0x%04x:", lid);
		for (port = 0; port < max_port; port++)
			fprintf(file, " %02x",
				osm_switch_get_hop_count(p_sw, lid, port));
		p_port = osm_get_port_by_lid_ho(&p_osm->subn, lid);
		if (p_port)
			fprintf(file, " # portguid 0x%016" PRIx64,
				cl_ntoh64(osm_port_get_guid(p_port)));
		fprintf(file, "\n");
	}
}