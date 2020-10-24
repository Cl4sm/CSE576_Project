static cl_status_t match_inf_rec(IN const cl_list_item_t * p_list_item,
				 IN void *context)
{
	osm_infr_t *p_infr_rec = (osm_infr_t *) context;
	osm_infr_t *p_infr = (osm_infr_t *) p_list_item;
	osm_log_t *p_log = p_infr_rec->sa->p_log;
	cl_status_t status = CL_NOT_FOUND;

	OSM_LOG_ENTER(p_log);

	if (memcmp(&p_infr->report_addr, &p_infr_rec->report_addr,
		   sizeof(p_infr_rec->report_addr))) {
		OSM_LOG(p_log, OSM_LOG_DEBUG, "Differ by Address\n");
		goto Exit;
	}

	/* if inform_info.gid is not zero, ignore lid range */
	if (ib_gid_is_notzero(&p_infr_rec->inform_record.inform_info.gid)) {
		if (memcmp(&p_infr->inform_record.inform_info.gid,
			   &p_infr_rec->inform_record.inform_info.gid,
			   sizeof(p_infr->inform_record.inform_info.gid))) {
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Differ by InformInfo.gid\n");
			goto Exit;
		}
	} else {
		if ((p_infr->inform_record.inform_info.lid_range_begin !=
		     p_infr_rec->inform_record.inform_info.lid_range_begin) ||
		    (p_infr->inform_record.inform_info.lid_range_end !=
		     p_infr_rec->inform_record.inform_info.lid_range_end)) {
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Differ by InformInfo.LIDRange\n");
			goto Exit;
		}
	}

	if (p_infr->inform_record.inform_info.trap_type !=
	    p_infr_rec->inform_record.inform_info.trap_type) {
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Differ by InformInfo.TrapType\n");
		goto Exit;
	}

	if (p_infr->inform_record.inform_info.is_generic !=
	    p_infr_rec->inform_record.inform_info.is_generic) {
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Differ by InformInfo.IsGeneric\n");
		goto Exit;
	}

	if (p_infr->inform_record.inform_info.is_generic) {
		if (p_infr->inform_record.inform_info.g_or_v.generic.trap_num !=
		    p_infr_rec->inform_record.inform_info.g_or_v.generic.
		    trap_num)
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Differ by InformInfo.Generic.TrapNumber\n");
		else if (p_infr->inform_record.inform_info.g_or_v.generic.
			 qpn_resp_time_val !=
			 p_infr_rec->inform_record.inform_info.g_or_v.generic.
			 qpn_resp_time_val)
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Differ by InformInfo.Generic.QPNRespTimeVal\n");
		else if (p_infr->inform_record.inform_info.g_or_v.generic.
			 node_type_msb !=
			 p_infr_rec->inform_record.inform_info.g_or_v.generic.
			 node_type_msb)
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Differ by InformInfo.Generic.NodeTypeMSB\n");
		else if (p_infr->inform_record.inform_info.g_or_v.generic.
			 node_type_lsb !=
			 p_infr_rec->inform_record.inform_info.g_or_v.generic.
			 node_type_lsb)
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Differ by InformInfo.Generic.NodeTypeLSB\n");
		else
			status = CL_SUCCESS;
	} else {
		if (p_infr->inform_record.inform_info.g_or_v.vend.dev_id !=
		    p_infr_rec->inform_record.inform_info.g_or_v.vend.dev_id)
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Differ by InformInfo.Vendor.DeviceID\n");
		else if (p_infr->inform_record.inform_info.g_or_v.vend.
			 qpn_resp_time_val !=
			 p_infr_rec->inform_record.inform_info.g_or_v.vend.
			 qpn_resp_time_val)
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Differ by InformInfo.Vendor.QPNRespTimeVal\n");
		else if (p_infr->inform_record.inform_info.g_or_v.vend.
			 vendor_id_msb !=
			 p_infr_rec->inform_record.inform_info.g_or_v.vend.
			 vendor_id_msb)
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Differ by InformInfo.Vendor.VendorIdMSB\n");
		else if (p_infr->inform_record.inform_info.g_or_v.vend.
			 vendor_id_lsb !=
			 p_infr_rec->inform_record.inform_info.g_or_v.vend.
			 vendor_id_lsb)
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Differ by InformInfo.Vendor.VendorIdLSB\n");
		else
			status = CL_SUCCESS;
	}

Exit:
	OSM_LOG_EXIT(p_log);
	return status;
}