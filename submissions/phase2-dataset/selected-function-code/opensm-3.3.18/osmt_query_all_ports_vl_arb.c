static ib_api_status_t
osmt_query_all_ports_vl_arb(IN osmtest_t * const p_osmt, IN FILE * fh)
{
	cl_status_t status = CL_SUCCESS;
	cl_qmap_t *p_tbl;
	port_t *p_src_port;
	uint8_t block, anyErr = 0;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Obtaining ALL Ports VL Arbitration Tables\n");

	/*
	 * Go over all ports that exist in the subnet
	 * get the relevant VLarbs
	 */

	p_tbl = &p_osmt->exp_subn.port_key_tbl;

	p_src_port = (port_t *) cl_qmap_head(p_tbl);

	while (p_src_port != (port_t *) cl_qmap_end(p_tbl)) {

		/* HACK we use capability_mask to know diff a CA port from switch port */
		if (p_src_port->rec.port_info.capability_mask) {
			/* this is an hca port */
			for (block = 1; block <= 4; block++) {
				/*  NOTE to comply we must set port number to 0 and the SA should figure it out */
				/*  since it is a CA port */
				status =
				    osmt_query_vl_arb(p_osmt,
						      p_src_port->rec.lid, 0,
						      block, fh);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0467: "
						"Failed to get Lid:0x%X Port:0x%X (%s)\n",
						cl_ntoh16(p_src_port->rec.lid),
						0, ib_get_err_str(status));
					anyErr = 1;
				}
			}
		} else {
			/* this is a switch port */
			for (block = 1; block <= 4; block++) {
				status =
				    osmt_query_vl_arb(p_osmt,
						      p_src_port->rec.lid,
						      p_src_port->rec.port_num,
						      block, fh);
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0468: "
						"Failed to get Lid:0x%X Port:0x%X (%s)\n",
						cl_ntoh16(p_src_port->rec.lid),
						p_src_port->rec.port_num,
						ib_get_err_str(status));
					anyErr = 1;
				}
			}
		}

		p_src_port = (port_t *) cl_qmap_next(&p_src_port->map_item);
	}

	OSM_LOG_EXIT(&p_osmt->log);
	if (anyErr) {
		status = IB_ERROR;
	}
	return (status);
}