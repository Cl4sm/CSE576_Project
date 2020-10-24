static ib_api_status_t
osmtest_get_sm_gid(IN osmtest_t * const p_osmt)
{
	cl_status_t status = IB_SUCCESS;
	node_t *p_node;
	const cl_qmap_t *p_tbl;
	ib_port_attr_t *local_port;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Calculate SM GID for use in subsequent SA queries\n");

	local_port = &p_osmt->local_port;
	p_osmt->sm_port_gid.unicast.prefix =
		cl_hton64(p_osmt->local_port_gid.unicast.prefix);

	if (local_port->lid != local_port->sm_lid) {
		p_tbl = &p_osmt->exp_subn.node_lid_tbl;
		p_node = (node_t *) cl_qmap_get(p_tbl, local_port->sm_lid);
		if (p_node == (node_t *) cl_qmap_end(p_tbl)) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				"ERR 0154: SM LID 0x%X doesn't exist\n",
				local_port->sm_lid);
			status = IB_ERROR;
			goto Exit;
		}
		p_osmt->sm_port_gid.unicast.interface_id = p_node->rec.node_info.port_guid;
	} else {
		p_osmt->sm_port_gid.unicast.interface_id =
			cl_hton64(p_osmt->local_port_gid.unicast.interface_id);
	}

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"SM GID 0x%016" PRIx64 " 0x%016" PRIx64 "\n",
		cl_ntoh64(p_osmt->sm_port_gid.unicast.prefix),
		cl_ntoh64(p_osmt->sm_port_gid.unicast.interface_id));

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}