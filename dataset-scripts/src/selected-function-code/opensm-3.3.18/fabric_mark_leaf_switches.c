static int fabric_mark_leaf_switches(IN ftree_fabric_t * p_ftree)
{
	ftree_sw_t *p_sw;
	ftree_hca_t *p_hca;
	ftree_hca_t *p_next_hca;
	unsigned i;
	int res = 0;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Marking leaf switches in fabric\n");

	/* Scan all the CAs, if they have CNs - find CN port and mark switch
	   that is connected to this port as leaf switch.
	   Also, ensure that this marked leaf has rank of p_ftree->leaf_switch_rank. */
	p_next_hca = (ftree_hca_t *) cl_qmap_head(&p_ftree->hca_tbl);
	while (p_next_hca != (ftree_hca_t *) cl_qmap_end(&p_ftree->hca_tbl)) {
		p_hca = p_next_hca;
		p_next_hca = (ftree_hca_t *) cl_qmap_next(&p_hca->map_item);
		if (!p_hca->cn_num)
			continue;

		for (i = 0; i < p_hca->up_port_groups_num; i++) {
			if (!p_hca->up_port_groups[i]->is_cn)
				continue;

			/* In CAs, port group alway has one port, and since this
			   port group is CN, we know that this port is compute node */
			CL_ASSERT(p_hca->up_port_groups[i]->remote_node_type ==
				  IB_NODE_TYPE_SWITCH);
			p_sw = p_hca->up_port_groups[i]->remote_hca_or_sw.p_sw;

			/* check if this switch was already processed */
			if (p_sw->is_leaf)
				continue;
			p_sw->is_leaf = TRUE;

			/* ensure that this leaf switch is at the correct tree level */
			if (p_sw->rank != p_ftree->leaf_switch_rank) {
				OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
					"ERR AB26: CN port 0x%" PRIx64
					" is connected to switch 0x%" PRIx64
					" with rank %u, "
					"while FatTree leaf rank is %u\n",
					cl_ntoh64(p_hca->
						  up_port_groups[i]->port_guid),
					sw_get_guid_ho(p_sw), p_sw->rank,
					p_ftree->leaf_switch_rank);
				res = -1;
				goto Exit;

			}
		}
	}

Exit:
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return res;
}				/* fabric_mark_leaf_switches() */