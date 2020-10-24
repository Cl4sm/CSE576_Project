static ib_api_status_t
osmtest_write_slvl_map_table(IN osmtest_t * const p_osmt,
			     IN FILE * fh,
			     IN const ib_slvl_table_record_t * const p_rec)
{
	int i;
	cl_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);

	fprintf(fh,
		"SLtoVL_MAP_TABLE\n"
		"lid                     0x%X\n"
		"in_port_num             0x%X\n"
		"out_port_num            0x%X\n",
		cl_ntoh16(p_rec->lid),
		p_rec->in_port_num, p_rec->out_port_num);

	fprintf(fh, "SL:");
	for (i = 0; i < 16; i++)
		fprintf(fh, "| %-2u  ", i);
	fprintf(fh, "|\nVL:");

	for (i = 0; i < 16; i++)
		fprintf(fh, "| 0x%01X ",
			ib_slvl_table_get(&p_rec->slvl_tbl, (uint8_t) i));
	fprintf(fh, "|\nEND\n\n");

	/*  Exit: */
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}