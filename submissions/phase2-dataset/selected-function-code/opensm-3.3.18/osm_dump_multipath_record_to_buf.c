static void osm_dump_multipath_record_to_buf(IN const ib_multipath_rec_t * p_mpr,
					     OUT char * buf)
{
	if (!buf || !p_mpr)
		return;
	else {
		char gid_str[INET6_ADDRSTRLEN];
		char buf_line[1024];
		ib_gid_t const *p_gid = p_mpr->gids;
		int i, n = 0;

		if (p_mpr->sgid_count) {
			for (i = 0; i < p_mpr->sgid_count; i++) {
				n += sprintf(buf_line + n,
					     "\t\t\t\tsgid%02d.................."
					     "%s\n", i + 1,
					     inet_ntop(AF_INET6, p_gid->raw,
						       gid_str,
						       sizeof gid_str));
				p_gid++;
			}
		}
		if (p_mpr->dgid_count) {
			for (i = 0; i < p_mpr->dgid_count; i++) {
				n += sprintf(buf_line + n,
					     "\t\t\t\tdgid%02d.................."
					     "%s\n", i + 1,
					     inet_ntop(AF_INET6, p_gid->raw,
						       gid_str,
						       sizeof gid_str));
				p_gid++;
			}
		}
		sprintf(buf,
			"MultiPath Record dump:\n"
			"\t\t\t\thop_flow_raw............0x%X\n"
			"\t\t\t\ttclass..................0x%X\n"
			"\t\t\t\tnum_path_revers.........0x%X\n"
			"\t\t\t\tpkey....................0x%X\n"
			"\t\t\t\tqos_class...............0x%X\n"
			"\t\t\t\tsl......................0x%X\n"
			"\t\t\t\tmtu.....................0x%X\n"
			"\t\t\t\trate....................0x%X\n"
			"\t\t\t\tpkt_life................0x%X\n"
			"\t\t\t\tindependence............0x%X\n"
			"\t\t\t\tsgid_count..............0x%X\n"
			"\t\t\t\tdgid_count..............0x%X\n"
			"\t\t\t\tservice_id..............0x%016" PRIx64 "\n"
			"%s\n",
			cl_ntoh32(p_mpr->hop_flow_raw), p_mpr->tclass,
			p_mpr->num_path, cl_ntoh16(p_mpr->pkey),
			ib_multipath_rec_qos_class(p_mpr),
			ib_multipath_rec_sl(p_mpr), p_mpr->mtu, p_mpr->rate,
			p_mpr->pkt_life, p_mpr->independence,
			p_mpr->sgid_count, p_mpr->dgid_count,
			cl_ntoh64(ib_multipath_rec_service_id(p_mpr)),
			buf_line);
	}
}