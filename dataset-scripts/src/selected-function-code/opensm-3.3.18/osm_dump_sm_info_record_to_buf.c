static void osm_dump_sm_info_record_to_buf(IN const ib_sminfo_record_t * p_smir,
					   OUT char * buf)
{
	if (!buf || !p_smir)
		return;
	else {
		sprintf(buf,
			"SMInfo Record dump:\n"
			"\t\t\t\tRID\n"
			"\t\t\t\tLid.....................%u\n"
			"\t\t\t\tReserved................0x%X\n"
			"\t\t\t\tSMInfo dump:\n"
			"\t\t\t\tguid....................0x%016" PRIx64 "\n"
			"\t\t\t\tsm_key..................0x%016" PRIx64 "\n"
			"\t\t\t\tact_count...............%u\n"
			"\t\t\t\tpriority................%u\n"
			"\t\t\t\tsm_state................%u\n",
			cl_ntoh16(p_smir->lid), cl_ntoh16(p_smir->resv0),
			cl_ntoh64(p_smir->sm_info.guid),
			cl_ntoh64(p_smir->sm_info.sm_key),
			cl_ntoh32(p_smir->sm_info.act_count),
			ib_sminfo_get_priority(&p_smir->sm_info),
			ib_sminfo_get_state(&p_smir->sm_info));
	}
}