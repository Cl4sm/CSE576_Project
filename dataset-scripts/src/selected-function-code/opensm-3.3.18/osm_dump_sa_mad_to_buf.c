static void osm_dump_sa_mad_to_buf(IN const ib_sa_mad_t * p_mad, OUT char * buf)
{
	if (!buf || !p_mad)
		return;
	else {
		/* make sure the mad is valid */
		if (p_mad == NULL) {
			sprintf(buf, "NULL MAD POINTER\n");
			return;
		}

		sprintf(buf,
			"SA MAD dump:\n"
			"\t\t\t\tbase_ver................0x%X\n"
			"\t\t\t\tmgmt_class..............0x%X\n"
			"\t\t\t\tclass_ver...............0x%X\n"
			"\t\t\t\tmethod..................0x%X (%s)\n"
			"\t\t\t\tstatus..................0x%X\n"
			"\t\t\t\tresv....................0x%X\n"
			"\t\t\t\ttrans_id................0x%" PRIx64 "\n"
			"\t\t\t\tattr_id.................0x%X (%s)\n"
			"\t\t\t\tresv1...................0x%X\n"
			"\t\t\t\tattr_mod................0x%X\n"
			"\t\t\t\trmpp_version............0x%X\n"
			"\t\t\t\trmpp_type...............0x%X\n"
			"\t\t\t\trmpp_flags..............0x%X\n"
			"\t\t\t\trmpp_status.............0x%X\n"
			"\t\t\t\tseg_num.................0x%X\n"
			"\t\t\t\tpayload_len/new_win.....0x%X\n"
			"\t\t\t\tsm_key..................0x%016" PRIx64 "\n"
			"\t\t\t\tattr_offset.............0x%X\n"
			"\t\t\t\tresv2...................0x%X\n"
			"\t\t\t\tcomp_mask...............0x%016" PRIx64 "\n",
			p_mad->base_ver, p_mad->mgmt_class, p_mad->class_ver,
			p_mad->method, ib_get_sa_method_str(p_mad->method),
			cl_ntoh16(p_mad->status), cl_ntoh16(p_mad->resv),
			cl_ntoh64(p_mad->trans_id), cl_ntoh16(p_mad->attr_id),
			ib_get_sa_attr_str(p_mad->attr_id),
			cl_ntoh16(p_mad->resv1), cl_ntoh32(p_mad->attr_mod),
			p_mad->rmpp_version, p_mad->rmpp_type,
			p_mad->rmpp_flags, p_mad->rmpp_status,
			cl_ntoh32(p_mad->seg_num),
			cl_ntoh32(p_mad->paylen_newwin),
			cl_ntoh64(p_mad->sm_key), cl_ntoh16(p_mad->attr_offset),
			cl_ntoh16(p_mad->resv3), cl_ntoh64(p_mad->comp_mask));

		strcat(buf, "\n");
	}
}