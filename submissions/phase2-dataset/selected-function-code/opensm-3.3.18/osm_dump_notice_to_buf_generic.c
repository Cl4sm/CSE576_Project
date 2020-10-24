static void osm_dump_notice_to_buf_generic(IN const ib_mad_notice_attr_t * p_ntci,
					   OUT char * log_buf)
{
	if (!log_buf || !p_ntci)
		return;
	else {
		char gid_str[INET6_ADDRSTRLEN];
		char gid_str2[INET6_ADDRSTRLEN];
		char buff[1024];
		int n;

		buff[0] = '\0';

		/* immediate data based on the trap */
		switch (cl_ntoh16(p_ntci->g_or_v.generic.trap_num)) {
		case 64:
		case 65:
		case 66:
		case 67:
			sprintf(buff,
				"\t\t\t\tsrc_gid..................%s\n",
				inet_ntop(AF_INET6, p_ntci->data_details.
					  ntc_64_67.gid.raw, gid_str,
					  sizeof gid_str));
			break;
		case 128:
			sprintf(buff,
				"\t\t\t\tsw_lid...................%u\n",
				cl_ntoh16(p_ntci->data_details.ntc_128.sw_lid));
			break;
		case 129:
		case 130:
		case 131:
			sprintf(buff,
				"\t\t\t\tlid......................%u\n"
				"\t\t\t\tport_num.................%u\n",
				cl_ntoh16(p_ntci->data_details.
					  ntc_129_131.lid),
				p_ntci->data_details.ntc_129_131.port_num);
			break;
		case 144:
			sprintf(buff,
				"\t\t\t\tlid......................%u\n"
				"\t\t\t\tlocal_changes............%u\n"
				"\t\t\t\tnew_cap_mask.............0x%08x\n"
				"\t\t\t\tchange_flags.............0x%x\n"
				"\t\t\t\tcap_mask2................0x%x\n",
				cl_ntoh16(p_ntci->data_details.ntc_144.lid),
				p_ntci->data_details.ntc_144.local_changes,
				cl_ntoh32(p_ntci->data_details.ntc_144.
					  new_cap_mask),
				cl_ntoh16(p_ntci->data_details.ntc_144.
					  change_flgs),
				cl_ntoh16(p_ntci->data_details.ntc_144.
					  cap_mask2));
			break;
		case 145:
			sprintf(buff,
				"\t\t\t\tlid......................%u\n"
				"\t\t\t\tnew_sys_guid.............0x%016"
				PRIx64 "\n",
				cl_ntoh16(p_ntci->data_details.ntc_145.
					  lid),
				cl_ntoh64(p_ntci->data_details.ntc_145.
					  new_sys_guid));
			break;
		case 256:
			n = sprintf(buff,
				    "\t\t\t\tlid......................%u\n"
				    "\t\t\t\tdrslid...................%u\n"
				    "\t\t\t\tmethod...................0x%x\n"
				    "\t\t\t\tattr_id..................0x%x\n"
				    "\t\t\t\tattr_mod.................0x%x\n"
				    "\t\t\t\tm_key....................0x%016"
				    PRIx64 "\n"
				    "\t\t\t\tdr_notice................%d\n"
				    "\t\t\t\tdr_path_truncated........%d\n"
				    "\t\t\t\tdr_hop_count.............%u\n",
				    cl_ntoh16(p_ntci->data_details.ntc_256.lid),
				    cl_ntoh16(p_ntci->data_details.ntc_256.
					      dr_slid),
				    p_ntci->data_details.ntc_256.method,
				    cl_ntoh16(p_ntci->data_details.ntc_256.
					      attr_id),
				    cl_ntoh32(p_ntci->data_details.ntc_256.
					      attr_mod),
				    cl_ntoh64(p_ntci->data_details.ntc_256.
					      mkey),
				    p_ntci->data_details.ntc_256.
				    dr_trunc_hop >> 7,
				    p_ntci->data_details.ntc_256.
				    dr_trunc_hop >> 6,
				    p_ntci->data_details.ntc_256.
				    dr_trunc_hop & 0x3f);
			n += snprintf(buff + n, sizeof(buff) - n,
				      "Directed Path Dump of %u hop path:"
				      "\n\t\t\t\tPath = ",
				      p_ntci->data_details.ntc_256.
				      dr_trunc_hop & 0x3f);
			n += sprint_uint8_arr(buff + n, sizeof(buff) - n,
					      p_ntci->data_details.ntc_256.
					      dr_rtn_path,
					      (p_ntci->data_details.ntc_256.
					       dr_trunc_hop & 0x3f) + 1);
			if (n >= sizeof(buff)) {
				n = sizeof(buff) - 2;
				break;
			}
			snprintf(buff + n, sizeof(buff) - n, "\n");
			break;
		case 257:
		case 258:
			sprintf(buff,
				"\t\t\t\tlid1.....................%u\n"
				"\t\t\t\tlid2.....................%u\n"
				"\t\t\t\tkey......................0x%x\n"
				"\t\t\t\tsl.......................%d\n"
				"\t\t\t\tqp1......................0x%x\n"
				"\t\t\t\tqp2......................0x%x\n"
				"\t\t\t\tgid1.....................%s\n"
				"\t\t\t\tgid2.....................%s\n",
				cl_ntoh16(p_ntci->data_details.ntc_257_258.
					  lid1),
				cl_ntoh16(p_ntci->data_details.ntc_257_258.
					  lid2),
				cl_ntoh32(p_ntci->data_details.ntc_257_258.key),
				cl_ntoh32(p_ntci->data_details.ntc_257_258.
					  qp1) >> 28,
				cl_ntoh32(p_ntci->data_details.ntc_257_258.
					  qp1) & 0xffffff,
				cl_ntoh32(p_ntci->data_details.ntc_257_258.
					  qp2) & 0xffffff,
				inet_ntop(AF_INET6, p_ntci->data_details.
					  ntc_257_258.gid1.raw, gid_str,
					  sizeof gid_str),
				inet_ntop(AF_INET6, p_ntci->data_details.
					  ntc_257_258.gid2.raw, gid_str2,
					  sizeof gid_str2));
			break;
		case 259:
			sprintf(buff,
				"\t\t\t\tdata_valid...............0x%x\n"
				"\t\t\t\tlid1.....................%u\n"
				"\t\t\t\tlid2.....................%u\n"
				"\t\t\t\tpkey.....................0x%x\n"
				"\t\t\t\tsl.......................%d\n"
				"\t\t\t\tqp1......................0x%x\n"
				"\t\t\t\tqp2......................0x%x\n"
				"\t\t\t\tgid1.....................%s\n"
				"\t\t\t\tgid2.....................%s\n"
				"\t\t\t\tsw_lid...................%u\n"
				"\t\t\t\tport_no..................%u\n",
				cl_ntoh16(p_ntci->data_details.ntc_259.
					  data_valid),
				cl_ntoh16(p_ntci->data_details.ntc_259.lid1),
				cl_ntoh16(p_ntci->data_details.ntc_259.lid2),
				cl_ntoh16(p_ntci->data_details.ntc_259.pkey),
				cl_ntoh32(p_ntci->data_details.ntc_259.
					  sl_qp1) >> 24,
				cl_ntoh32(p_ntci->data_details.ntc_259.
					  sl_qp1) & 0xffffff,
				cl_ntoh32(p_ntci->data_details.ntc_259.qp2),
				inet_ntop(AF_INET6, p_ntci->data_details.
					  ntc_259.gid1.raw, gid_str,
					  sizeof gid_str),
				inet_ntop(AF_INET6, p_ntci->data_details.
					  ntc_259.gid2.raw, gid_str2,
					  sizeof gid_str2),
				cl_ntoh16(p_ntci->data_details.ntc_259.sw_lid),
				p_ntci->data_details.ntc_259.port_no);
			break;
		}

		sprintf(log_buf,
			"Generic Notice dump:\n"
			"\t\t\t\ttype.....................%u\n"
			"\t\t\t\tprod_type................%u (%s)\n"
			"\t\t\t\ttrap_num.................%u\n%s",
			ib_notice_get_type(p_ntci),
			cl_ntoh32(ib_notice_get_prod_type(p_ntci)),
			ib_get_producer_type_str(ib_notice_get_prod_type
						 (p_ntci)),
			cl_ntoh16(p_ntci->g_or_v.generic.trap_num), buff);
	}
}