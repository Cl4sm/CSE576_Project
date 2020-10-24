ib_api_status_t osm_get_gid_by_mad_addr(IN osm_log_t * p_log,
					IN const osm_subn_t * p_subn,
					IN osm_mad_addr_t * p_mad_addr,
					OUT ib_gid_t * p_gid)
{
	const osm_port_t *p_port;

	if (p_gid == NULL) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 7505: "
			"Provided output GID is NULL\n");
		return IB_INVALID_PARAMETER;
	}

	p_port = osm_get_port_by_mad_addr(p_log, p_subn, p_mad_addr);
	if (!p_port)
		return IB_INVALID_PARAMETER;

	p_gid->unicast.interface_id = p_port->p_physp->port_guid;
	p_gid->unicast.prefix = p_subn->opt.subnet_prefix;

	return IB_SUCCESS;
}