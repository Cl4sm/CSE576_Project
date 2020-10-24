static void sa_dump_one_service(cl_list_item_t * p_list_item, void *cxt)
{
	FILE *file = ((struct opensm_dump_context *)cxt)->file;
	osm_svcr_t *p_svcr = (osm_svcr_t *) p_list_item;
	ib_service_record_t *p_sr = &p_svcr->service_record;

	fprintf(file, "Service Record: id=0x%016" PRIx64
		" gid=0x%016" PRIx64 ":0x%016" PRIx64
		" pkey=0x%x"
		" lease=0x%x"
		" key=0x%02x%02x%02x%02x%02x%02x%02x%02x"
		":0x%02x%02x%02x%02x%02x%02x%02x%02x"
		" name=\'%s\'"
		" data8=0x%02x%02x%02x%02x%02x%02x%02x%02x"
		":0x%02x%02x%02x%02x%02x%02x%02x%02x"
		" data16=0x%04x%04x%04x%04x:0x%04x%04x%04x%04x"
		" data32=0x%08x%08x:0x%08x%08x"
		" data64=0x%016" PRIx64 ":0x%016" PRIx64
		" modified_time=0x%x lease_period=0x%x\n\n",
		cl_ntoh64(p_sr->service_id),
		cl_ntoh64(p_sr->service_gid.unicast.prefix),
		cl_ntoh64(p_sr->service_gid.unicast.interface_id),
		cl_ntoh16(p_sr->service_pkey),
		cl_ntoh32(p_sr->service_lease),
		p_sr->service_key[0], p_sr->service_key[1],
		p_sr->service_key[2], p_sr->service_key[3],
		p_sr->service_key[4], p_sr->service_key[5],
		p_sr->service_key[6], p_sr->service_key[7],
		p_sr->service_key[8], p_sr->service_key[9],
		p_sr->service_key[10], p_sr->service_key[11],
		p_sr->service_key[12], p_sr->service_key[13],
		p_sr->service_key[14], p_sr->service_key[15],
		p_sr->service_name,
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
		cl_ntoh64(p_sr->service_data64[1]),
		p_svcr->modified_time, p_svcr->lease_period);
}