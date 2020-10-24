static ib_api_status_t
osmtest_write_link(IN osmtest_t * const p_osmt,
		   IN FILE * fh, IN const ib_link_record_t * const p_rec)
{
	int result;
	cl_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);

	result = fprintf(fh,
			 "DEFINE_LINK\n"
			 "from_lid                0x%X\n"
			 "from_port_num           0x%X\n"
			 "to_port_num             0x%X\n"
			 "to_lid                  0x%X\n"
			 "END\n\n",
			 cl_ntoh16(p_rec->from_lid),
			 p_rec->from_port_num,
			 p_rec->to_port_num, cl_ntoh16(p_rec->to_lid));

	if (result < 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0164: "
			"Write failed\n");
		status = IB_ERROR;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}