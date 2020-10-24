static void osm_dump_inform_info_to_buf_generic(IN const ib_inform_info_t * p_ii,
						OUT char * buf)
{
	if (!buf || !p_ii)
		return;
	else {
		uint32_t qpn;
		uint8_t resp_time_val;
		char gid_str[INET6_ADDRSTRLEN];

		ib_inform_info_get_qpn_resp_time(p_ii->g_or_v.generic.
						 qpn_resp_time_val, &qpn,
						 &resp_time_val);
		sprintf(buf,
			"InformInfo dump:\n"
			"\t\t\t\tgid.....................%s\n"
			"\t\t\t\tlid_range_begin.........%u\n"
			"\t\t\t\tlid_range_end...........%u\n"
			"\t\t\t\tis_generic..............0x%X\n"
			"\t\t\t\tsubscribe...............0x%X\n"
			"\t\t\t\ttrap_type...............0x%X\n"
			"\t\t\t\ttrap_num................%u\n"
			"\t\t\t\tqpn.....................0x%06X\n"
			"\t\t\t\tresp_time_val...........0x%X\n"
			"\t\t\t\tnode_type...............0x%06X\n" "",
			inet_ntop(AF_INET6, p_ii->gid.raw, gid_str,
				  sizeof gid_str),
			cl_ntoh16(p_ii->lid_range_begin),
			cl_ntoh16(p_ii->lid_range_end),
			p_ii->is_generic, p_ii->subscribe,
			cl_ntoh16(p_ii->trap_type),
			cl_ntoh16(p_ii->g_or_v.generic.trap_num),
			cl_ntoh32(qpn), resp_time_val,
			cl_ntoh32(ib_inform_info_get_prod_type(p_ii)));
	}
}