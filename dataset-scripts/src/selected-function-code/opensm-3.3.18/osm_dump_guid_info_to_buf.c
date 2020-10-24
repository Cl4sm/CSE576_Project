static void osm_dump_guid_info_to_buf(IN ib_net64_t node_guid,
				      IN ib_net64_t port_guid,
				      IN uint8_t block_num,
				      IN const ib_guid_info_t * p_gi,
				      OUT char * buf)
{
	if (!buf || !p_gi)
		return;
	else {
		sprintf(buf,
			"GUIDInfo dump:\n"
			"\t\t\t\tblock number............%u\n"
			"\t\t\t\tnode_guid...............0x%016" PRIx64 "\n"
			"\t\t\t\tport_guid...............0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 0..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 1..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 2..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 3..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 4..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 5..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 6..................0x%016" PRIx64 "\n"
			"\t\t\t\tGUID 7..................0x%016" PRIx64 "\n",
			block_num, cl_ntoh64(node_guid), cl_ntoh64(port_guid),
			cl_ntoh64(p_gi->guid[0]), cl_ntoh64(p_gi->guid[1]),
			cl_ntoh64(p_gi->guid[2]), cl_ntoh64(p_gi->guid[3]),
			cl_ntoh64(p_gi->guid[4]), cl_ntoh64(p_gi->guid[5]),
			cl_ntoh64(p_gi->guid[6]), cl_ntoh64(p_gi->guid[7]));
	}
}