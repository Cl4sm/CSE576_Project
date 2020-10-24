static void osm_dump_inform_info_record_to_buf(IN const ib_inform_info_record_t * p_iir,
					       OUT char * buf)
{
	if(!buf || p_iir)
		return;
	else {
		char gid_str[INET6_ADDRSTRLEN];
		char gid_str2[INET6_ADDRSTRLEN];
		uint32_t qpn;
		uint8_t resp_time_val;

		ib_inform_info_get_qpn_resp_time(p_iir->inform_info.g_or_v.
						 generic.qpn_resp_time_val,
						 &qpn, &resp_time_val);
		sprintf(buf,
			"InformInfo Record dump:\n"
			"\t\t\t\tRID\n"
			"\t\t\t\tSubscriberGID...........%s\n"
			"\t\t\t\tSubscriberEnum..........0x%X\n"
			"\t\t\t\tInformInfo dump:\n"
			"\t\t\t\tgid.....................%s\n"
			"\t\t\t\tlid_range_begin.........%u\n"
			"\t\t\t\tlid_range_end...........%u\n"
			"\t\t\t\tis_generic..............0x%X\n"
			"\t\t\t\tsubscribe...............0x%X\n"
			"\t\t\t\ttrap_type...............0x%X\n"
			"\t\t\t\tdev_id..................0x%X\n"
			"\t\t\t\tqpn.....................0x%06X\n"
			"\t\t\t\tresp_time_val...........0x%X\n"
			"\t\t\t\tvendor_id...............0x%06X\n" "",
			inet_ntop(AF_INET6, p_iir->subscriber_gid.raw,
				  gid_str, sizeof gid_str),
			cl_ntoh16(p_iir->subscriber_enum),
			inet_ntop(AF_INET6, p_iir->inform_info.gid.raw,
				  gid_str2, sizeof gid_str2),
			cl_ntoh16(p_iir->inform_info.lid_range_begin),
			cl_ntoh16(p_iir->inform_info.lid_range_end),
			p_iir->inform_info.is_generic,
			p_iir->inform_info.subscribe,
			cl_ntoh16(p_iir->inform_info.trap_type),
			cl_ntoh16(p_iir->inform_info.g_or_v.vend.
				  dev_id), cl_ntoh32(qpn),
			resp_time_val,
			cl_ntoh32(ib_inform_info_get_prod_type
				  (&p_iir->inform_info)));
	}
}