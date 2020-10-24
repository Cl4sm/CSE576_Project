static ib_api_status_t
osmtest_write_vl_arb_table(IN osmtest_t * const p_osmt,
			   IN FILE * fh,
			   IN const ib_vl_arb_table_record_t * const p_rec)
{
	int i;
	cl_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);

	fprintf(fh,
		"VL_ARBITRATION_TABLE\n"
		"lid                     0x%X\n"
		"port_num                0x%X\n"
		"block                   0x%X\n",
		cl_ntoh16(p_rec->lid),
		p_rec->port_num, p_rec->block_num);

	fprintf(fh, "       ");
	for (i = 0; i < 32; i++)
		fprintf(fh, "| %-2u ", i);
	fprintf(fh, "|\nVL:    ");

	for (i = 0; i < 32; i++)
		fprintf(fh, "|0x%02X", p_rec->vl_arb_tbl.vl_entry[i].vl);
	fprintf(fh, "|\nWEIGHT:");

	for (i = 0; i < 32; i++)
		fprintf(fh, "|0x%02X", p_rec->vl_arb_tbl.vl_entry[i].weight);
	fprintf(fh, "|\nEND\n\n");

	/*  Exit: */
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}