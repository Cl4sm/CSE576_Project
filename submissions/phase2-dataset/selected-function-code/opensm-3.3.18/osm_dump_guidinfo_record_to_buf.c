static void osm_dump_guidinfo_record_to_buf(IN const ib_guidinfo_record_t * p_gir,
					    OUT char * buf)
{
	if (!buf || !p_gir)
		return;
	else {
		const ib_guid_info_t *p_gi = &p_gir->guid_info;

		sprintf(buf,
			"GUIDInfo Record dump:\n"
			"\t\t\t\tRID\n"
			"\t\t\t\tLid.....................%u\n"
			"\t\t\t\tBlockNum................0x%X\n"
			"\t\t\t\tReserved................0x%X\n"
			"\t\t\t\tGUIDInfo dump:\n"
			"\t\t\t\tReserved................0x%X\n"
			"\t\t\t\tGUID 0..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 1..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 2..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 3..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 4..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 5..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 6..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 7..................0x%016" PRIx64 "\n",
			cl_ntoh16(p_gir->lid), p_gir->block_num, p_gir->resv,
			cl_ntoh32(p_gir->reserved),
			cl_ntoh64(p_gi->guid[0]), cl_ntoh64(p_gi->guid[1]),
			cl_ntoh64(p_gi->guid[2]), cl_ntoh64(p_gi->guid[3]),
			cl_ntoh64(p_gi->guid[4]), cl_ntoh64(p_gi->guid[5]),
			cl_ntoh64(p_gi->guid[6]), cl_ntoh64(p_gi->guid[7]));
	}
}