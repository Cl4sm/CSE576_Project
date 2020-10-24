static void log_trap_info(osm_log_t *p_log, ib_mad_notice_attr_t *p_ntci,
			  ib_net16_t source_lid, ib_net64_t trans_id)
{
	if (!OSM_LOG_IS_ACTIVE_V2(p_log, OSM_LOG_ERROR))
		return;

	if (ib_notice_is_generic(p_ntci)) {
		char str[32];

		if ((p_ntci->g_or_v.generic.trap_num == CL_HTON16(SM_LINK_INTEGRITY_THRESHOLD_TRAP)) ||
		    (p_ntci->g_or_v.generic.trap_num == CL_HTON16(SM_BUFFER_OVERRUN_THRESHOLD_TRAP)) ||
		    (p_ntci->g_or_v.generic.trap_num == CL_HTON16(SM_WATCHDOG_TIMER_EXPIRED_TRAP)))
			snprintf(str, sizeof(str), " Port %u",
				 p_ntci->data_details.ntc_129_131.port_num);
		else
			str[0] = '\0';

		OSM_LOG(p_log, OSM_LOG_ERROR,
			"Received Generic Notice type:%u "
			"num:%u (%s) Producer:%u (%s) "
			"from LID:%u%s TID:0x%016" PRIx64 "\n",
			ib_notice_get_type(p_ntci),
			cl_ntoh16(p_ntci->g_or_v.generic.trap_num),
			ib_get_trap_str(p_ntci->g_or_v.generic.trap_num),
			cl_ntoh32(ib_notice_get_prod_type(p_ntci)),
			ib_get_producer_type_str(ib_notice_get_prod_type(p_ntci)),
			cl_hton16(source_lid), str, cl_ntoh64(trans_id));
		if ((p_ntci->g_or_v.generic.trap_num == CL_HTON16(SM_BAD_PKEY_TRAP)) ||
		    (p_ntci->g_or_v.generic.trap_num == CL_HTON16(SM_BAD_QKEY_TRAP))) {
			OSM_LOG(p_log, OSM_LOG_ERROR,
				"Bad %s_Key:0x%x on SL:%d from "
				"LID1:%u QP1:0x%x to "
				"LID2:%u QP2:0x%x\n",
				(p_ntci->g_or_v.generic.trap_num == CL_HTON16(257)) ? "P" : "Q",
				cl_ntoh32(p_ntci->data_details.ntc_257_258.key),
				cl_ntoh32(p_ntci->data_details.ntc_257_258.qp1) >> 28,
				cl_ntoh16(p_ntci->data_details.ntc_257_258.lid1),
				cl_ntoh32(p_ntci->data_details.ntc_257_258.qp1) & 0xfff,
				cl_ntoh16(p_ntci->data_details.ntc_257_258.lid2),
				cl_ntoh32(p_ntci->data_details.ntc_257_258.qp2));
		}
	} else
		OSM_LOG(p_log, OSM_LOG_ERROR,
			"Received Vendor Notice type:%u vend:0x%06X "
			"dev:%u from LID:%u TID:0x%016" PRIx64 "\n",
			ib_notice_get_type(p_ntci),
			cl_ntoh32(ib_notice_get_vend_id(p_ntci)),
			cl_ntoh16(p_ntci->g_or_v.vend.dev_id),
			cl_ntoh16(source_lid), cl_ntoh64(trans_id));
}