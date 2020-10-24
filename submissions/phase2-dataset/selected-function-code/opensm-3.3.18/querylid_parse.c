static void querylid_parse(char **p_last, osm_opensm_t * p_osm, FILE * out)
{
	unsigned int p = 0;
	uint16_t lid = 0;
	osm_port_t *p_port = NULL;
	char *p_cmd = next_token(p_last);

	if (!p_cmd) {
		fprintf(out, "no LID specified\n");
		help_querylid(out, 1);
		return;
	}

	lid = (uint16_t) strtoul(p_cmd, NULL, 0);
	cl_plock_acquire(&p_osm->lock);
	p_port = osm_get_port_by_lid_ho(&p_osm->subn, lid);
	if (!p_port)
		goto invalid_lid;

	fprintf(out, "Query results for LID %u\n", lid);
	fprintf(out,
		"   GUID                : 0x%016" PRIx64 "\n"
		"   Node Desc           : %s\n"
		"   Node Type           : %s\n"
		"   Num Ports           : %d\n",
		cl_ntoh64(p_port->guid),
		p_port->p_node->print_desc,
		ib_get_node_type_str(osm_node_get_type(p_port->p_node)),
		p_port->p_node->node_info.num_ports);

	if (p_port->p_node->sw)
		p = 0;
	else
		p = 1;
	for ( /* see above */ ; p < p_port->p_node->physp_tbl_size; p++) {
		fprintf(out,
			"   Port %u health       : %s\n",
			p,
			p_port->p_node->physp_table[p].
			healthy ? "OK" : "ERROR");
	}

	cl_plock_release(&p_osm->lock);
	return;

invalid_lid:
	cl_plock_release(&p_osm->lock);
	fprintf(out, "Invalid lid %d\n", lid);
	return;
}