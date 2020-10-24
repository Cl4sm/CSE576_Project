static void fabric_set_leaf_rank(IN ftree_fabric_t * p_ftree)
{
	unsigned i;
	ftree_sw_t *p_sw;
	ftree_hca_t *p_hca = NULL;
	ftree_hca_t *p_next_hca;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	if (!fabric_roots_provided(p_ftree)) {
		/* If root file is not provided, the fabric has to be pure fat-tree
		   in terms of ranking. Thus, leaf switches rank is the max rank. */
		p_ftree->leaf_switch_rank = p_ftree->max_switch_rank;
	} else {
		/* Find the first CN and set the leaf_switch_rank to the rank
		   of the switch that is connected to this CN. Later we will
		   ensure that all the leaf switches have the same rank. */
		p_next_hca = (ftree_hca_t *) cl_qmap_head(&p_ftree->hca_tbl);
		while (p_next_hca !=
		       (ftree_hca_t *) cl_qmap_end(&p_ftree->hca_tbl)) {
			p_hca = p_next_hca;
			if (p_hca->cn_num)
				break;
			p_next_hca =
			    (ftree_hca_t *) cl_qmap_next(&p_hca->map_item);
		}
		/* we know that there are CNs in the fabric, so just to be sure... */
		CL_ASSERT(p_next_hca !=
			  (ftree_hca_t *) cl_qmap_end(&p_ftree->hca_tbl));

		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
			"Selected CN port GUID 0x%" PRIx64 "\n",
			hca_get_guid_ho(p_hca));

		for (i = 0; (i < p_hca->up_port_groups_num)
		     && (!p_hca->up_port_groups[i]->is_cn); i++) ;
		CL_ASSERT(i < p_hca->up_port_groups_num);
		CL_ASSERT(p_hca->up_port_groups[i]->remote_node_type ==
			  IB_NODE_TYPE_SWITCH);

		p_sw = p_hca->up_port_groups[i]->remote_hca_or_sw.p_sw;
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
			"Selected leaf switch GUID 0x%" PRIx64 ", rank %u\n",
			sw_get_guid_ho(p_sw), p_sw->rank);
		p_ftree->leaf_switch_rank = p_sw->rank;
	}

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"FatTree leaf switch rank is %u\n", p_ftree->leaf_switch_rank);
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
}				/* fabric_set_leaf_rank() */