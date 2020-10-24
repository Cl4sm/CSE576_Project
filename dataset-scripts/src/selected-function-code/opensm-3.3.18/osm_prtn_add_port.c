ib_api_status_t osm_prtn_add_port(osm_log_t * p_log, osm_subn_t * p_subn,
				  osm_prtn_t * p, ib_net64_t guid,
				  boolean_t full)
{
	ib_api_status_t status = IB_SUCCESS;
	cl_map_t *p_tbl;
	osm_port_t *p_port;
	osm_physp_t *p_physp;

	p_port = osm_get_port_by_guid(p_subn, guid);
	if (!p_port) {
		OSM_LOG(p_log, OSM_LOG_VERBOSE,
			"port 0x%" PRIx64 " not found\n", cl_ntoh64(guid));
		return status;
	}

	p_physp = p_port->p_physp;
	if (!p_physp) {
		OSM_LOG(p_log, OSM_LOG_VERBOSE,
			"no physical for port 0x%" PRIx64 "\n",
			cl_ntoh64(guid));
		return status;
	}

	p_tbl = (full == TRUE) ? &p->full_guid_tbl : &p->part_guid_tbl;

	if (p_subn->opt.allow_both_pkeys) {
		if (cl_map_remove(p_tbl, guid))
			OSM_LOG(p_log, OSM_LOG_ERROR, "port 0x%" PRIx64
				" already in partition \'%s\' (0x%04x) full %d."
				" Will overwrite\n",
				cl_ntoh64(guid), p->name, cl_ntoh16(p->pkey),
				full);
	} else {
		if (cl_map_remove(&p->part_guid_tbl, guid) ||
		    cl_map_remove(&p->full_guid_tbl, guid))
			OSM_LOG(p_log, OSM_LOG_VERBOSE, "port 0x%" PRIx64
				" already in partition \'%s\' (0x%04x)."
				" Will overwrite\n",
				cl_ntoh64(guid), p->name, cl_ntoh16(p->pkey));
	}

	if (cl_map_insert(p_tbl, guid, p_physp) == NULL)
		return IB_INSUFFICIENT_MEMORY;

	return status;
}