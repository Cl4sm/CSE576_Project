static void dump_sl2vl_tbl(cl_map_item_t * item, FILE * file, void *cxt)
{
	osm_port_t *p_port = (osm_port_t *) item;
	osm_node_t *p_node = p_port->p_node;
	uint32_t in_port, out_port,
		 num_ports = p_node->node_info.num_ports;
	ib_net16_t base_lid = osm_port_get_base_lid(p_port);
	osm_physp_t *p_physp;
	ib_slvl_table_t *p_tbl;
	int i, n;
	char buf[1024];
	const char * header_line =	"#in out : 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15";
	const char * separator_line = "#--------------------------------------------------------";

	if (!num_ports)
		return;

	fprintf(file, "%s 0x%016" PRIx64 ", base LID %d, "
		"\"%s\"\n%s\n%s\n",
		ib_get_node_type_str(p_node->node_info.node_type),
		cl_ntoh64(p_port->guid), cl_ntoh16(base_lid),
		p_node->print_desc, header_line, separator_line);

	if (p_node->node_info.node_type == IB_NODE_TYPE_SWITCH) {
		for (out_port = 0; out_port <= num_ports; out_port++){
			p_physp = osm_node_get_physp_ptr(p_node, out_port);

			/* no need to print SL2VL table for port that is down */
			if (!p_physp->p_remote_physp)
				continue;

			for (in_port = 0; in_port <= num_ports; in_port++) {
				p_tbl = osm_physp_get_slvl_tbl(p_physp, in_port);
				for (i = 0, n = 0; i < 16; i++)
					n += sprintf(buf + n, " %-2d",
						ib_slvl_table_get(p_tbl, i));
				fprintf(file, "%-3d %-3d :%s\n",
					in_port, out_port, buf);
			}
		}
	} else {
		p_physp = p_port->p_physp;
		p_tbl = osm_physp_get_slvl_tbl(p_physp, 0);
		for (i = 0, n = 0; i < 16; i++)
			n += sprintf(buf + n, " %-2d",
					ib_slvl_table_get(p_tbl, i));
		fprintf(file, "%-3d %-3d :%s\n", 0, 0, buf);
	}

	fprintf(file, "%s\n\n", separator_line);
}