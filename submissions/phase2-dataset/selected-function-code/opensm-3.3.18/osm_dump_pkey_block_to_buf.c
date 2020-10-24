static void osm_dump_pkey_block_to_buf(IN uint64_t port_guid,
				       IN uint16_t block_num,
				       IN uint8_t port_num,
				       IN const ib_pkey_table_t * p_pkey_tbl,
				       OUT char * buf)
{
	if (!buf || !p_pkey_tbl)
		return;
	else {
		char buf_line[1024];
		int i, n;

		for (i = 0, n = 0; i < 32; i++)
			n += sprintf(buf_line + n, " 0x%04x |",
				     cl_ntoh16(p_pkey_tbl->pkey_entry[i]));

		sprintf(buf,
			"P_Key table dump:\n"
			"\t\t\tport_guid...........0x%016" PRIx64 "\n"
			"\t\t\tblock_num...........0x%X\n"
			"\t\t\tport_num............%u\n\tP_Key Table: %s\n",
			cl_ntoh64(port_guid), block_num, port_num, buf_line);
	}
}