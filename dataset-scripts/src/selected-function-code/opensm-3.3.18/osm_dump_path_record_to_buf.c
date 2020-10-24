static void osm_dump_path_record_to_buf(IN const ib_path_rec_t * p_pr,
					OUT char * buf)
{
	if (!buf || !p_pr)
		return;
	else {
		char gid_str[INET6_ADDRSTRLEN];
		char gid_str2[INET6_ADDRSTRLEN];

		sprintf(buf,
			"PathRecord dump:\n"
			"\t\t\t\tservice_id..............0x%016" PRIx64 "\n"
			"\t\t\t\tdgid....................%s\n"
			"\t\t\t\tsgid....................%s\n"
			"\t\t\t\tdlid....................%u\n"
			"\t\t\t\tslid....................%u\n"
			"\t\t\t\thop_flow_raw............0x%X\n"
			"\t\t\t\ttclass..................0x%X\n"
			"\t\t\t\tnum_path_revers.........0x%X\n"
			"\t\t\t\tpkey....................0x%X\n"
			"\t\t\t\tqos_class...............0x%X\n"
			"\t\t\t\tsl......................0x%X\n"
			"\t\t\t\tmtu.....................0x%X\n"
			"\t\t\t\trate....................0x%X\n"
			"\t\t\t\tpkt_life................0x%X\n"
			"\t\t\t\tpreference..............0x%X\n"
			"\t\t\t\tresv2...................0x%02X%02X%02X%02X%02X%02X\n",
			cl_ntoh64(p_pr->service_id),
			inet_ntop(AF_INET6, p_pr->dgid.raw, gid_str,
				  sizeof gid_str),
			inet_ntop(AF_INET6, p_pr->sgid.raw, gid_str2,
				  sizeof gid_str2),
			cl_ntoh16(p_pr->dlid), cl_ntoh16(p_pr->slid),
			cl_ntoh32(p_pr->hop_flow_raw), p_pr->tclass,
			p_pr->num_path, cl_ntoh16(p_pr->pkey),
			ib_path_rec_qos_class(p_pr), ib_path_rec_sl(p_pr),
			p_pr->mtu, p_pr->rate, p_pr->pkt_life, p_pr->preference,
			p_pr->resv2[0], p_pr->resv2[1], p_pr->resv2[2],
			p_pr->resv2[3], p_pr->resv2[4], p_pr->resv2[5]);
	}
}