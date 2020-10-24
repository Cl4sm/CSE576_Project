static void portstatus_parse(char **p_last, osm_opensm_t * p_osm, FILE * out)
{
	fabric_stats_t fs;
	struct timeval before, after;
	char *p_cmd;

	memset(&fs, 0, sizeof(fs));

	p_cmd = next_token(p_last);
	if (p_cmd) {
		if (strcmp(p_cmd, "ca") == 0) {
			fs.node_type_lim = IB_NODE_TYPE_CA;
		} else if (strcmp(p_cmd, "switch") == 0) {
			fs.node_type_lim = IB_NODE_TYPE_SWITCH;
		} else if (strcmp(p_cmd, "router") == 0) {
			fs.node_type_lim = IB_NODE_TYPE_ROUTER;
		} else {
			fprintf(out, "Node type not understood\n");
			help_portstatus(out, 1);
			return;
		}
	}

	gettimeofday(&before, NULL);

	/* for each node in the system gather the stats */
	cl_plock_acquire(&p_osm->lock);
	cl_qmap_apply_func(&(p_osm->subn.node_guid_tbl), __get_stats,
			   (void *)&fs);
	cl_plock_release(&p_osm->lock);

	gettimeofday(&after, NULL);

	/* report the stats */
	fprintf(out, "\"%s\" port status:\n",
		fs.node_type_lim ? ib_get_node_type_str(fs.
							node_type_lim) : "ALL");
	fprintf(out,
		"   %" PRIu64 " port(s) scanned on %" PRIu64
		" nodes in %lu us\n", fs.total_ports, fs.total_nodes,
		after.tv_usec - before.tv_usec);

	if (fs.ports_down)
		fprintf(out, "   %" PRIu64 " down\n", fs.ports_down);
	if (fs.ports_active)
		fprintf(out, "   %" PRIu64 " active\n", fs.ports_active);
	if (fs.ports_1X)
		fprintf(out, "   %" PRIu64 " at 1X\n", fs.ports_1X);
	if (fs.ports_4X)
		fprintf(out, "   %" PRIu64 " at 4X\n", fs.ports_4X);
	if (fs.ports_8X)
		fprintf(out, "   %" PRIu64 " at 8X\n", fs.ports_8X);
	if (fs.ports_12X)
		fprintf(out, "   %" PRIu64 " at 12X\n", fs.ports_12X);

	if (fs.ports_sdr)
		fprintf(out, "   %" PRIu64 " at 2.5 Gbps\n", fs.ports_sdr);
	if (fs.ports_ddr)
		fprintf(out, "   %" PRIu64 " at 5.0 Gbps\n", fs.ports_ddr);
	if (fs.ports_qdr)
		fprintf(out, "   %" PRIu64 " at 10.0 Gbps\n", fs.ports_qdr);
	if (fs.ports_fdr10)
		fprintf(out, "   %" PRIu64 " at 10.0 Gbps (FDR10)\n", fs.ports_fdr10);
	if (fs.ports_fdr)
		fprintf(out, "   %" PRIu64 " at 14.0625 Gbps\n", fs.ports_fdr);
	if (fs.ports_edr)
		fprintf(out, "   %" PRIu64 " at 25.78125 Gbps\n", fs.ports_edr);

	if (fs.ports_disabled + fs.ports_reduced_speed + fs.ports_reduced_width
	    + fs.ports_unenabled_width + fs.ports_unenabled_speed > 0) {
		fprintf(out, "\nPossible issues:\n");
	}
	if (fs.ports_disabled) {
		fprintf(out, "   %" PRIu64 " disabled\n", fs.ports_disabled);
		__print_port_report(out, fs.disabled_ports);
	}
	if (fs.ports_unenabled_speed) {
		fprintf(out, "   %" PRIu64 " with unenabled speed\n",
			fs.ports_unenabled_speed);
		__print_port_report(out, fs.unenabled_speed_ports);
	}
	if (fs.ports_reduced_speed) {
		fprintf(out, "   %" PRIu64 " with reduced speed\n",
			fs.ports_reduced_speed);
		__print_port_report(out, fs.reduced_speed_ports);
	}
	if (fs.ports_unenabled_width) {
		fprintf(out, "   %" PRIu64 " with unenabled width\n",
			fs.ports_unenabled_width);
		__print_port_report(out, fs.unenabled_width_ports);
	}
	if (fs.ports_reduced_width) {
		fprintf(out, "   %" PRIu64 " with reduced width\n",
			fs.ports_reduced_width);
		__print_port_report(out, fs.reduced_width_ports);
	}
	fprintf(out, "\n");
}