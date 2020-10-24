static int dnup_lid_matrices(void *ctx)
{
	dnup_t *p_dnup = ctx;
	cl_map_item_t *item;
	osm_switch_t *p_sw;
	int ret = 0;
	int num_leafs = 0;
	uint8_t pn, pn_rem;

	OSM_LOG_ENTER(&p_dnup->p_osm->log);

	for (item = cl_qmap_head(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *)item;
		p_sw->priv = create_dnup_node(p_sw);
		if (!p_sw->priv) {
			OSM_LOG(&(p_dnup->p_osm->log), OSM_LOG_ERROR, "ERR AE0C: "
				"cannot create dnup node\n");
			OSM_LOG_EXIT(&p_dnup->p_osm->log);
			return -1;
		}
	}


	/* First setup node level nodes */
	for (item = cl_qmap_head(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *)item;

		for (pn = 0; pn < p_sw->num_ports; pn++) {
			osm_node_t *p_remote_node;
			p_remote_node = osm_node_get_remote_node(p_sw->p_node, pn, &pn_rem);
			if(p_remote_node && !p_remote_node->sw) {
				struct dnup_node *u = p_sw->priv;
				u->rank = 0;
				OSM_LOG(&(p_dnup->p_osm->log),
					OSM_LOG_VERBOSE, "(%s) rank 0 leaf switch\n",
					p_sw->p_node->print_desc);
				num_leafs++;
				break;
			}
		}
	}

	if(num_leafs == 0) {
		OSM_LOG(&(p_dnup->p_osm->log),
			OSM_LOG_ERROR, "ERR AE0D: No leaf switches found, DnUp routing failed\n");
		OSM_LOG_EXIT(&p_dnup->p_osm->log);
		return -1;
	}

	ret = dnup_build_lid_matrices(p_dnup);

	for (item = cl_qmap_head(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *) item;
		delete_dnup_node(p_sw->priv);
	}

	OSM_LOG_EXIT(&p_dnup->p_osm->log);
	return ret;
}