ib_api_status_t osm_prtn_add_all(osm_log_t * p_log, osm_subn_t * p_subn,
				 osm_prtn_t * p, unsigned type, boolean_t full)
{
	cl_qmap_t *p_port_tbl = &p_subn->port_guid_tbl;
	cl_map_item_t *p_item;
	osm_port_t *p_port;
	ib_api_status_t status = IB_SUCCESS;

	p_item = cl_qmap_head(p_port_tbl);
	while (p_item != cl_qmap_end(p_port_tbl)) {
		p_port = (osm_port_t *) p_item;
		p_item = cl_qmap_next(p_item);
		if (!type || osm_node_get_type(p_port->p_node) == type) {
			status = osm_prtn_add_port(p_log, p_subn, p,
						   osm_port_get_guid(p_port),
						   full);
			if (status != IB_SUCCESS)
				goto _err;
		}
	}

_err:
	return status;
}