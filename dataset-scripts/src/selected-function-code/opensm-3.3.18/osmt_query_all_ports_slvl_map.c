static ib_api_status_t
osmt_query_all_ports_slvl_map(IN osmtest_t * const p_osmt, IN FILE * fh)
{
	cl_status_t status = CL_SUCCESS;
	cl_qmap_t *p_tbl;
	port_t *p_src_port;
	uint8_t in_port, anyErr = 0, num_ports;
	node_t *p_node;
	const cl_qmap_t *p_node_tbl;

	OSM_LOG_ENTER(&p_osmt->log);

	/*
	 * Go over all ports that exist in the subnet
	 * get the relevant SLtoVLs
	 */

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Obtaining ALL Ports (to other ports) SLtoVL Maps\n");

	p_tbl = &p_osmt->exp_subn.port_key_tbl;
	p_node_tbl = &p_osmt->exp_subn.node_lid_tbl;

	p_src_port = (port_t *) cl_qmap_head(p_tbl);

	while (p_src_port != (port_t *) cl_qmap_end(p_tbl)) {

		/* HACK we use capability_mask to know diff a CA port from switch port */
		if (p_src_port->rec.port_info.capability_mask) {
			/* this is an hca port */
			/*  NOTE to comply we must set port number to 0 and the SA should figure it out */
			/*  since it is a CA port */
			status =
			    osmt_query_slvl_map(p_osmt, p_src_port->rec.lid, 0,
						0, fh);
			if (status != IB_SUCCESS) {
				OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0471: "
					"Failed to get Lid:0x%X In-Port:0x%X Out-Port:0x%X(%s)\n",
					cl_ntoh16(p_src_port->rec.lid), 0, 0,
					ib_get_err_str(status));
				anyErr = 1;
			}
		} else {
			/* this is a switch port */
			/* get the node */
			p_node =
			    (node_t *) cl_qmap_get(p_node_tbl,
						   p_src_port->rec.lid);
			if (p_node == (node_t *) cl_qmap_end(p_node_tbl)) {
				OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0472: "
					"Failed to get Node by Lid:0x%X\n",
					p_src_port->rec.lid);
				goto Exit;
			}

			num_ports = p_node->rec.node_info.num_ports;

			for (in_port = 1; in_port <= num_ports; in_port++) {
				status =
				    osmt_query_slvl_map(p_osmt,
							p_src_port->rec.lid,
							p_src_port->rec.
							port_num, in_port, fh);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0473: "
						"Failed to get Lid:0x%X In-Port:0x%X Out-Port:0x%X (%s)\n",
						cl_ntoh16(p_src_port->rec.lid),
						p_src_port->rec.port_num,
						in_port,
						ib_get_err_str(status));
					anyErr = 1;
				}
			}
		}

		p_src_port = (port_t *) cl_qmap_next(&p_src_port->map_item);
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	if (anyErr) {
		status = IB_ERROR;
	}
	return (status);
}