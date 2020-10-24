static void fabric_dump_general_info(IN ftree_fabric_t * p_ftree)
{
	uint32_t i, j;
	ftree_sw_t *p_sw;

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_INFO,
		"General fabric topology info\n");
	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_INFO,
		"============================\n");

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_INFO,
		"  - FatTree rank (roots to leaf switches): %u\n",
		p_ftree->leaf_switch_rank + 1);
	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_INFO,
		"  - FatTree max switch rank: %u\n", p_ftree->max_switch_rank);
	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_INFO,
		"  - Fabric has %u CAs, %u CA ports (%u of them CNs), %u switches\n",
		cl_qmap_count(&p_ftree->hca_tbl), p_ftree->ca_ports,
		p_ftree->cn_num, cl_qmap_count(&p_ftree->sw_tbl));

	CL_ASSERT(p_ftree->ca_ports >= p_ftree->cn_num);

	for (i = 0; i <= p_ftree->max_switch_rank; i++) {
		j = 0;
		for (p_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
		     p_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl);
		     p_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item)) {
			if (p_sw->rank == i)
				j++;
		}
		if (i == 0)
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_INFO,
				"  - Fabric has %u switches at rank %u (roots)\n",
				j, i);
		else if (i == p_ftree->leaf_switch_rank)
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_INFO,
				"  - Fabric has %u switches at rank %u (%u of them leafs)\n",
				j, i, p_ftree->leaf_switches_num);
		else
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_INFO,
				"  - Fabric has %u switches at rank %u\n", j,
				i);
	}

	if (OSM_LOG_IS_ACTIVE_V2(&p_ftree->p_osm->log, OSM_LOG_VERBOSE)) {
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
			"  - Root switches:\n");
		for (p_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
		     p_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl);
		     p_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item)) {
			if (p_sw->rank == 0)
				OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
					"      GUID: 0x%016" PRIx64
					", LID: %u, Index %s\n",
					sw_get_guid_ho(p_sw),
					p_sw->base_lid,
					tuple_to_str(p_sw->tuple));
		}

		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
			"  - Leaf switches (sorted by index):\n");
		for (i = 0; i < p_ftree->leaf_switches_num; i++) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
				"      GUID: 0x%016" PRIx64
				", LID: %u, Index %s\n",
				sw_get_guid_ho(p_ftree->leaf_switches[i]),
				p_ftree->leaf_switches[i]->base_lid,
				tuple_to_str(p_ftree->leaf_switches[i]->tuple));
		}
	}
}				/* fabric_dump_general_info() */