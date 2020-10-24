static void osm_dump_vl_arb_table_to_buf(IN uint64_t port_guid,
					 IN uint8_t block_num,
					 IN uint8_t port_num,
					 IN const ib_vl_arb_table_t * p_vla_tbl,
					 OUT char * buf)
{
	if (!buf || !p_vla_tbl)
		return;
	else {
		char buf_line1[1024], buf_line2[1024];
		int i, n;

		for (i = 0, n = 0; i < 32; i++)
			n += sprintf(buf_line1 + n, " 0x%01X |",
				     p_vla_tbl->vl_entry[i].vl);
		for (i = 0, n = 0; i < 32; i++)
			n += sprintf(buf_line2 + n, " 0x%01X |",
				     p_vla_tbl->vl_entry[i].weight);
		sprintf(buf,
			"VLArb dump:\n" "\t\t\tport_guid...........0x%016"
			PRIx64 "\n" "\t\t\tblock_num...........0x%X\n"
			"\t\t\tport_num............%u\n\tVL    : | %s\n\tWEIGHT:| %s\n",
			cl_ntoh64(port_guid), block_num, port_num, buf_line1,
			buf_line2);
	}
}