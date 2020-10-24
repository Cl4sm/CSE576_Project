static ib_api_status_t osmtest_check_missing_ports(IN osmtest_t * const p_osmt)
{
	const port_t *p_port;
	cl_status_t status = IB_SUCCESS;
	cl_qmap_t *p_tbl;

	OSM_LOG_ENTER(&p_osmt->log);

	p_tbl = &p_osmt->exp_subn.port_key_tbl;

	p_port = (port_t *) cl_qmap_head(p_tbl);

	while (p_port != (port_t *) cl_qmap_end(p_tbl)) {
		if (p_port->count == 0) {
			/*
			 * This port was not reported by the SA
			 */
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0081: "
				"Missing port LID:0x%X Num:0x%X\n",
				cl_ntoh16(p_port->rec.lid),
				p_port->rec.port_num);
			status = IB_ERROR;
		}

		p_port = (port_t *) cl_qmap_next(&p_port->map_item);
	}

	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}