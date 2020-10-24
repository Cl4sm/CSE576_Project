static void osm_dump_slvl_map_table_to_buf(IN uint64_t port_guid,
					   IN uint8_t in_port_num,
					   IN uint8_t out_port_num,
					   IN const ib_slvl_table_t * p_slvl_tbl,
					   OUT char * buf)
{
	if (!buf || !p_slvl_tbl)
		return;
	else {
		char buf_line1[1024], buf_line2[1024];
		int n;
		uint8_t i;

		for (i = 0, n = 0; i < 16; i++)
			n += sprintf(buf_line1 + n, " %-2u |", i);
		for (i = 0, n = 0; i < 16; i++)
			n += sprintf(buf_line2 + n, "0x%01X |",
				     ib_slvl_table_get(p_slvl_tbl, i));
		sprintf(buf,
			"SLtoVL dump:\n"
			"\t\t\tport_guid............0x%016" PRIx64 "\n"
			"\t\t\tin_port_num..........%u\n"
			"\t\t\tout_port_num.........%u\n\tSL: | %s\n\tVL: | %s\n",
			cl_ntoh64(port_guid), in_port_num, out_port_num,
			buf_line1, buf_line2);
	}
}