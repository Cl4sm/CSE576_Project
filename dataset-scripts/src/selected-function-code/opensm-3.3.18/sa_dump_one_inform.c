static void sa_dump_one_inform(cl_list_item_t * p_list_item, void *cxt)
{
	FILE *file = ((struct opensm_dump_context *)cxt)->file;
	osm_infr_t *p_infr = (osm_infr_t *) p_list_item;
	ib_inform_info_record_t *p_iir = &p_infr->inform_record;

	fprintf(file, "InformInfo Record:"
		" subscriber_gid=0x%016" PRIx64 ":0x%016" PRIx64
		" subscriber_enum=0x%x"
		" InformInfo:"
		" gid=0x%016" PRIx64 ":0x%016" PRIx64
		" lid_range_begin=0x%x"
		" lid_range_end=0x%x"
		" is_generic=0x%x"
		" subscribe=0x%x"
		" trap_type=0x%x"
		" trap_num=0x%x"
		" qpn_resp_time_val=0x%x"
		" node_type=0x%06x"
		" rep_addr: lid=0x%04x path_bits=0x%02x static_rate=0x%02x"
		" remote_qp=0x%08x remote_qkey=0x%08x pkey_ix=0x%04x sl=0x%02x"
		"\n\n",
		cl_ntoh64(p_iir->subscriber_gid.unicast.prefix),
		cl_ntoh64(p_iir->subscriber_gid.unicast.interface_id),
		cl_ntoh16(p_iir->subscriber_enum),
		cl_ntoh64(p_iir->inform_info.gid.unicast.prefix),
		cl_ntoh64(p_iir->inform_info.gid.unicast.interface_id),
		cl_ntoh16(p_iir->inform_info.lid_range_begin),
		cl_ntoh16(p_iir->inform_info.lid_range_end),
		p_iir->inform_info.is_generic,
		p_iir->inform_info.subscribe,
		cl_ntoh16(p_iir->inform_info.trap_type),
		cl_ntoh16(p_iir->inform_info.g_or_v.generic.trap_num),
		cl_ntoh32(p_iir->inform_info.g_or_v.generic.qpn_resp_time_val),
		cl_ntoh32(ib_inform_info_get_prod_type(&p_iir->inform_info)),
		cl_ntoh16(p_infr->report_addr.dest_lid),
		p_infr->report_addr.path_bits,
		p_infr->report_addr.static_rate,
		cl_ntoh32(p_infr->report_addr.addr_type.gsi.remote_qp),
		cl_ntoh32(p_infr->report_addr.addr_type.gsi.remote_qkey),
		p_infr->report_addr.addr_type.gsi.pkey_ix,
		p_infr->report_addr.addr_type.gsi.service_level);
}