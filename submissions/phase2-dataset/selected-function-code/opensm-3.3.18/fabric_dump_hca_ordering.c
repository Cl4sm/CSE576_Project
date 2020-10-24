static void fabric_dump_hca_ordering(IN ftree_fabric_t * p_ftree)
{
	ftree_hca_t *p_hca;
	ftree_sw_t *p_sw;
	ftree_port_group_t *p_group_on_sw;
	ftree_port_group_t *p_group_on_hca;
	uint32_t i;
	uint32_t j;
	unsigned printed_hcas_on_leaf;

	char path[1024];
	FILE *p_hca_ordering_file;
	const char *filename = "opensm-ftree-ca-order.dump";

	snprintf(path, sizeof(path), "%s/%s",
		 p_ftree->p_osm->subn.opt.dump_files_dir, filename);
	p_hca_ordering_file = fopen(path, "w");
	if (!p_hca_ordering_file) {
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR, "ERR AB01: "
			"cannot open file \'%s\': %s\n", filename,
			strerror(errno));
		return;
	}

	/* for each leaf switch (in indexing order) */
	for (i = 0; i < p_ftree->leaf_switches_num; i++) {
		p_sw = p_ftree->leaf_switches[i];
		printed_hcas_on_leaf = 0;

		/* for each real CA (CNs and not) connected to this switch */
		for (j = 0; j < p_sw->down_port_groups_num; j++) {
			p_group_on_sw = p_sw->down_port_groups[j];

			if (p_group_on_sw->remote_node_type != IB_NODE_TYPE_CA)
				continue;

			p_hca = p_group_on_sw->remote_hca_or_sw.p_hca;
			p_group_on_hca =
			    hca_get_port_group_by_remote_lid(p_hca,
							     p_group_on_sw->
							     base_lid);

			/* treat non-compute nodes as dummies */
			if (!p_group_on_hca->is_cn)
				continue;

			fprintf(p_hca_ordering_file, "0x%04x\t%s\n",
				p_group_on_hca->base_lid,
				p_hca->p_osm_node->print_desc);

			printed_hcas_on_leaf++;
		}

		/* now print missing HCAs */
		for (j = 0;
		     j < (p_ftree->max_cn_per_leaf - printed_hcas_on_leaf); j++)
			fprintf(p_hca_ordering_file, "0xFFFF\tDUMMY\n");

	}
	/* done going through all the leaf switches */

	fclose(p_hca_ordering_file);
}				/* fabric_dump_hca_ordering() */