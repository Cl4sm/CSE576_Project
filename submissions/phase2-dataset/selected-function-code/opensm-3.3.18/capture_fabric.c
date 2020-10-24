static
bool capture_fabric(struct fabric *fabric)
{
	osm_subn_t *subnet = &fabric->osm->subn;
	osm_switch_t *osm_sw;
	osm_physp_t *lphysp, *rphysp;
	osm_port_t *lport;
	osm_node_t *osm_node;
	cl_map_item_t *item;
	uint8_t ltype, rtype;
	int p, port_cnt;
	guid_t sw_guid;
	bool success = true;

	OSM_LOG_ENTER(&fabric->osm->log);

	/*
	 * On OpenSM data structures:
	 *
	 * Apparently, every port in a fabric has an associated osm_physp_t,
	 * but not every port has an associated osm_port_t.  Apparently every
	 * osm_port_t has an associated osm_physp_t.
	 *
	 * So, in order to find the inter-switch links we need to walk the
	 * switch list and examine each port, via its osm_physp_t object.
	 *
	 * But, we need to associate our CA and switch management port
	 * endpoints with the corresponding osm_port_t objects, in order
	 * to simplify computation of LFT entries and perform SL lookup for
	 * path records. Since it is apparently difficult to locate the
	 * osm_port_t that corresponds to a given osm_physp_t, we also
	 * need to walk the list of ports indexed by GUID to get access
	 * to the appropriate osm_port_t objects.
	 *
	 * Need to allocate our switches before we do anything else.
	 */
	item = cl_qmap_head(&subnet->sw_guid_tbl);
	while (item != cl_qmap_end(&subnet->sw_guid_tbl)) {

		osm_sw = (osm_switch_t *)item;
		item = cl_qmap_next(item);
		osm_sw->priv = NULL;  /* avoid stale pointer dereferencing */
		osm_node = osm_sw->p_node;

		if (osm_node_get_type(osm_node) != IB_NODE_TYPE_SWITCH)
			continue;

		port_cnt = osm_node_get_num_physp(osm_node);
		sw_guid = osm_node_get_node_guid(osm_node);

		success = alloc_fswitch(fabric, sw_guid, port_cnt);
		if (!success)
			goto out;
	}
	/*
	 * Now build all our endpoints.
	 */
	item = cl_qmap_head(&subnet->port_guid_tbl);
	while (item != cl_qmap_end(&subnet->port_guid_tbl)) {

		lport = (osm_port_t *)item;
		item = cl_qmap_next(item);
		lport->priv = NULL;  /* avoid stale pointer dereferencing */

		lphysp = lport->p_physp;
		if (!(lphysp && osm_physp_is_valid(lphysp)))
			continue;

		ltype = osm_node_get_type(lphysp->p_node);
		/*
		 * Switch management port is always port 0.
		 */
		if (lphysp->port_num == 0 && ltype == IB_NODE_TYPE_SWITCH) {
			success = build_sw_endpoint(fabric, lport);
			if (!success)
				goto out;
			continue;
		}
		rphysp = lphysp->p_remote_physp;
		if (!(rphysp && osm_physp_is_valid(rphysp)))
			continue;

		rtype = osm_node_get_type(rphysp->p_node);

		if ((ltype != IB_NODE_TYPE_CA &&
		     ltype != IB_NODE_TYPE_ROUTER) ||
		    rtype != IB_NODE_TYPE_SWITCH)
			continue;

		success =
			build_ca_link(fabric, lport,
				      osm_node_get_node_guid(rphysp->p_node),
				      osm_physp_get_port_num(rphysp));
		if (!success)
			goto out;
	}
	/*
	 * Lastly, build all our interswitch links.
	 */
	item = cl_qmap_head(&subnet->sw_guid_tbl);
	while (item != cl_qmap_end(&subnet->sw_guid_tbl)) {

		osm_sw = (osm_switch_t *)item;
		item = cl_qmap_next(item);

		port_cnt = osm_node_get_num_physp(osm_sw->p_node);
		for (p = 0; p < port_cnt; p++) {

			lphysp = osm_node_get_physp_ptr(osm_sw->p_node, p);
			if (!(lphysp && osm_physp_is_valid(lphysp)))
				continue;

			rphysp = lphysp->p_remote_physp;
			if (!(rphysp && osm_physp_is_valid(rphysp)))
				continue;

			if (lphysp == rphysp)
				continue;	/* ignore loopbacks */

			ltype = osm_node_get_type(lphysp->p_node);
			rtype = osm_node_get_type(rphysp->p_node);

			if (ltype != IB_NODE_TYPE_SWITCH ||
			    rtype != IB_NODE_TYPE_SWITCH)
				continue;

			success =
				build_link(fabric,
					   osm_node_get_node_guid(lphysp->p_node),
					   osm_physp_get_port_num(lphysp),
					   osm_node_get_node_guid(rphysp->p_node),
					   osm_physp_get_port_num(rphysp));
			if (!success)
				goto out;
		}
	}
out:
	OSM_LOG_EXIT(&fabric->osm->log);
	return success;
}