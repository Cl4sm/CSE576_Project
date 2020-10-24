static void osm_dump_service_record_to_buf(IN const ib_service_record_t * p_sr,
					   OUT char * buf)
{
	if (!buf || !p_sr)
		return;
	else {
		char gid_str[INET6_ADDRSTRLEN];
		char buf_service_key[35];
		char buf_service_name[65];

		sprintf(buf_service_key,
			"0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			p_sr->service_key[0], p_sr->service_key[1],
			p_sr->service_key[2], p_sr->service_key[3],
			p_sr->service_key[4], p_sr->service_key[5],
			p_sr->service_key[6], p_sr->service_key[7],
			p_sr->service_key[8], p_sr->service_key[9],
			p_sr->service_key[10], p_sr->service_key[11],
			p_sr->service_key[12], p_sr->service_key[13],
			p_sr->service_key[14], p_sr->service_key[15]);
		strncpy(buf_service_name, (char *)p_sr->service_name, 64);
		buf_service_name[64] = '\0';

		sprintf(buf,
			"Service Record dump:\n"
			"\t\t\t\tServiceID...............0x%016" PRIx64 "\n"
			"\t\t\t\tServiceGID..............%s\n"
			"\t\t\t\tServiceP_Key............0x%X\n"
			"\t\t\t\tServiceLease............0x%X\n"
			"\t\t\t\tServiceKey..............%s\n"
			"\t\t\t\tServiceName.............%s\n"
			"\t\t\t\tServiceData8.1..........0x%X\n"
			"\t\t\t\tServiceData8.2..........0x%X\n"
			"\t\t\t\tServiceData8.3..........0x%X\n"
			"\t\t\t\tServiceData8.4..........0x%X\n"
			"\t\t\t\tServiceData8.5..........0x%X\n"
			"\t\t\t\tServiceData8.6..........0x%X\n"
			"\t\t\t\tServiceData8.7..........0x%X\n"
			"\t\t\t\tServiceData8.8..........0x%X\n"
			"\t\t\t\tServiceData8.9..........0x%X\n"
			"\t\t\t\tServiceData8.10.........0x%X\n"
			"\t\t\t\tServiceData8.11.........0x%X\n"
			"\t\t\t\tServiceData8.12.........0x%X\n"
			"\t\t\t\tServiceData8.13.........0x%X\n"
			"\t\t\t\tServiceData8.14.........0x%X\n"
			"\t\t\t\tServiceData8.15.........0x%X\n"
			"\t\t\t\tServiceData8.16.........0x%X\n"
			"\t\t\t\tServiceData16.1.........0x%X\n"
			"\t\t\t\tServiceData16.2.........0x%X\n"
			"\t\t\t\tServiceData16.3.........0x%X\n"
			"\t\t\t\tServiceData16.4.........0x%X\n"
			"\t\t\t\tServiceData16.5.........0x%X\n"
			"\t\t\t\tServiceData16.6.........0x%X\n"
			"\t\t\t\tServiceData16.7.........0x%X\n"
			"\t\t\t\tServiceData16.8.........0x%X\n"
			"\t\t\t\tServiceData32.1.........0x%X\n"
			"\t\t\t\tServiceData32.2.........0x%X\n"
			"\t\t\t\tServiceData32.3.........0x%X\n"
			"\t\t\t\tServiceData32.4.........0x%X\n"
			"\t\t\t\tServiceData64.1.........0x%016" PRIx64 "\n"
			"\t\t\t\tServiceData64.2.........0x%016" PRIx64 "\n",
			cl_ntoh64(p_sr->service_id),
			inet_ntop(AF_INET6, p_sr->service_gid.raw, gid_str,
				  sizeof gid_str),
			cl_ntoh16(p_sr->service_pkey),
			cl_ntoh32(p_sr->service_lease),
			buf_service_key, buf_service_name,
			p_sr->service_data8[0], p_sr->service_data8[1],
			p_sr->service_data8[2], p_sr->service_data8[3],
			p_sr->service_data8[4], p_sr->service_data8[5],
			p_sr->service_data8[6], p_sr->service_data8[7],
			p_sr->service_data8[8], p_sr->service_data8[9],
			p_sr->service_data8[10], p_sr->service_data8[11],
			p_sr->service_data8[12], p_sr->service_data8[13],
			p_sr->service_data8[14], p_sr->service_data8[15],
			cl_ntoh16(p_sr->service_data16[0]),
			cl_ntoh16(p_sr->service_data16[1]),
			cl_ntoh16(p_sr->service_data16[2]),
			cl_ntoh16(p_sr->service_data16[3]),
			cl_ntoh16(p_sr->service_data16[4]),
			cl_ntoh16(p_sr->service_data16[5]),
			cl_ntoh16(p_sr->service_data16[6]),
			cl_ntoh16(p_sr->service_data16[7]),
			cl_ntoh32(p_sr->service_data32[0]),
			cl_ntoh32(p_sr->service_data32[1]),
			cl_ntoh32(p_sr->service_data32[2]),
			cl_ntoh32(p_sr->service_data32[3]),
			cl_ntoh64(p_sr->service_data64[0]),
			cl_ntoh64(p_sr->service_data64[1]));
	}
}