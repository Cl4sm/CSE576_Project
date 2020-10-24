static void updn_find_root_nodes_by_min_hop(OUT updn_t * p_updn)
{
	osm_opensm_t *p_osm = p_updn->p_osm;
	osm_switch_t *p_sw;
	osm_port_t *p_port;
	osm_physp_t *p_physp;
	cl_map_item_t *item;
	double thd1, thd2;
	unsigned i, cas_num = 0;
	unsigned *cas_per_sw;
	uint16_t lid_ho;

	OSM_LOG_ENTER(&p_osm->log);

	OSM_LOG(&p_osm->log, OSM_LOG_DEBUG,
		"Current number of ports in the subnet is %d\n",
		cl_qmap_count(&p_osm->subn.port_guid_tbl));

	lid_ho = (uint16_t) cl_ptr_vector_get_size(&p_updn->p_osm->subn.port_lid_tbl) + 1;
	cas_per_sw = malloc(lid_ho * sizeof(*cas_per_sw));
	if (!cas_per_sw) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR, "ERR AA14: "
			"cannot alloc mem for CAs per switch counter array\n");
		goto _exit;
	}
	memset(cas_per_sw, 0, lid_ho * sizeof(*cas_per_sw));

	/* Find the Maximum number of CAs (and routers) for histogram normalization */
	OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
		"Finding the number of CAs and storing them in cl_map\n");
	for (item = cl_qmap_head(&p_updn->p_osm->subn.port_guid_tbl);
	     item != cl_qmap_end(&p_updn->p_osm->subn.port_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_port = (osm_port_t *)item;
		if (!p_port->p_node->sw) {
			p_physp = p_port->p_physp->p_remote_physp;
			if (!p_physp || !p_physp->p_node->sw)
				continue;
			lid_ho = osm_node_get_base_lid(p_physp->p_node, 0);
			lid_ho = cl_ntoh16(lid_ho);
			cas_per_sw[lid_ho]++;
			cas_num++;
		}
	}

	thd1 = cas_num * 0.9;
	thd2 = cas_num * 0.05;
	OSM_LOG(&p_osm->log, OSM_LOG_DEBUG,
		"Found %u CAs and RTRs, %u SWs in the subnet. "
		"Thresholds are thd1 = %f && thd2 = %f\n",
		cas_num, cl_qmap_count(&p_osm->subn.sw_guid_tbl), thd1, thd2);

	OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
		"Passing through all switches to collect Min Hop info\n");
	for (item = cl_qmap_head(&p_updn->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_updn->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		unsigned hop_hist[IB_SUBNET_PATH_HOPS_MAX];
		uint16_t max_lid_ho;
		uint8_t hop_val;
		uint16_t numHopBarsOverThd1 = 0;
		uint16_t numHopBarsOverThd2 = 0;

		p_sw = (osm_switch_t *) item;

		memset(hop_hist, 0, sizeof(hop_hist));

		max_lid_ho = p_sw->max_lid_ho;
		for (lid_ho = 1; lid_ho <= max_lid_ho; lid_ho++)
			if (cas_per_sw[lid_ho]) {
				hop_val =
				    osm_switch_get_least_hops(p_sw, lid_ho);
				if (hop_val >= IB_SUBNET_PATH_HOPS_MAX)
					continue;

				hop_hist[hop_val] += cas_per_sw[lid_ho];
			}

		/* Now recognize the spines by requiring one bar to be
		   above 90% of the number of CAs and RTRs */
		for (i = 0; i < IB_SUBNET_PATH_HOPS_MAX; i++) {
			if (hop_hist[i] > thd1)
				numHopBarsOverThd1++;
			if (hop_hist[i] > thd2)
				numHopBarsOverThd2++;
		}

		/* If thd conditions are valid - rank the root node */
		if (numHopBarsOverThd1 == 1 && numHopBarsOverThd2 == 1) {
			OSM_LOG(&p_osm->log, OSM_LOG_DEBUG,
				"Ranking GUID 0x%" PRIx64 " as root node\n",
				cl_ntoh64(osm_node_get_node_guid(p_sw->p_node)));
			((struct updn_node *)p_sw->priv)->rank = 0;
			p_updn->num_roots++;
		}
	}

	free(cas_per_sw);
_exit:
	OSM_LOG_EXIT(&p_osm->log);
	return;
}