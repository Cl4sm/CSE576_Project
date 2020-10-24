static void log_notice(osm_log_t * log, osm_log_level_t level,
		       ib_mad_notice_attr_t * ntc)
{
	char gid_str[INET6_ADDRSTRLEN], gid_str2[INET6_ADDRSTRLEN];
	ib_gid_t *gid;
	ib_gid_t *gid1, *gid2;

	/* an official Event information log */
	if (ib_notice_is_generic(ntc)) {
		if ((ntc->g_or_v.generic.trap_num == CL_HTON16(SM_GID_IN_SERVICE_TRAP)) ||
		    (ntc->g_or_v.generic.trap_num == CL_HTON16(SM_GID_OUT_OF_SERVICE_TRAP)) ||
		    (ntc->g_or_v.generic.trap_num == CL_HTON16(SM_MGID_CREATED_TRAP)) ||
		    (ntc->g_or_v.generic.trap_num == CL_HTON16(SM_MGID_DESTROYED_TRAP)))
			gid = &ntc->data_details.ntc_64_67.gid;
		else
			gid = &ntc->issuer_gid;

		switch (cl_ntoh16(ntc->g_or_v.generic.trap_num)) {
		case SM_GID_IN_SERVICE_TRAP:
		case SM_GID_OUT_OF_SERVICE_TRAP:
			OSM_LOG(log, level,
				"Reporting Informational Notice \"%s\", GID:%s\n",
				ib_get_trap_str(ntc->g_or_v.generic.trap_num),
				inet_ntop(AF_INET6, gid->raw, gid_str, sizeof gid_str));
			break;
		case SM_MGID_CREATED_TRAP:
		case SM_MGID_DESTROYED_TRAP:
			OSM_LOG(log, level,
				"Reporting Informational Notice \"%s\", MGID:%s\n",
				ib_get_trap_str(ntc->g_or_v.generic.trap_num),
				inet_ntop(AF_INET6, gid->raw, gid_str, sizeof gid_str));
			break;
		case SM_UNPATH_TRAP:
		case SM_REPATH_TRAP:
			/* TODO: Fill in details once SM starts to use these traps */
			OSM_LOG(log, level,
				"Reporting Informational Notice \"%s\"n",
				ib_get_trap_str(ntc->g_or_v.generic.trap_num));
			break;
		case SM_LINK_STATE_CHANGED_TRAP:
			OSM_LOG(log, level,
				"Reporting Urgent Notice \"%s\" from switch LID %u, "
				"GUID 0x%016" PRIx64 "\n",
				ib_get_trap_str(ntc->g_or_v.generic.trap_num),
				cl_ntoh16(ntc->issuer_lid),
				cl_ntoh64(gid->unicast.interface_id));
			break;
		case SM_LINK_INTEGRITY_THRESHOLD_TRAP:
		case SM_BUFFER_OVERRUN_THRESHOLD_TRAP:
		case SM_WATCHDOG_TIMER_EXPIRED_TRAP:
			OSM_LOG(log, level,
				"Reporting Urgent Notice \"%s\" from LID %u, "
				"GUID 0x%016" PRIx64 ", port %u\n",
				ib_get_trap_str(ntc->g_or_v.generic.trap_num),
				cl_ntoh16(ntc->issuer_lid),
				cl_ntoh64(gid->unicast.interface_id),
				ntc->data_details.ntc_129_131.port_num);
			break;
		case SM_LOCAL_CHANGES_TRAP:
			if (ntc->data_details.ntc_144.local_changes == 1)
				OSM_LOG(log, level,
					"Reporting Informational Notice \"%s\" from LID %u, "
					"GUID 0x%016" PRIx64 ", ChangeFlags 0x%04x, "
					"CapabilityMask2 0x%04x\n",
					ib_get_trap_str(ntc->g_or_v.generic.trap_num),
					cl_ntoh16(ntc->issuer_lid),
					cl_ntoh64(gid->unicast.interface_id),
					cl_ntoh16(ntc->data_details.ntc_144.change_flgs),
					cl_ntoh16(ntc->data_details.ntc_144.cap_mask2));
			else
				OSM_LOG(log, level,
					"Reporting Informational Notice \"%s\" from LID %u, "
					"GUID 0x%016" PRIx64 ", new CapabilityMask 0x%08x\n",
					ib_get_trap_str(ntc->g_or_v.generic.trap_num),
					cl_ntoh16(ntc->issuer_lid),
					cl_ntoh64(gid->unicast.interface_id),
					cl_ntoh32(ntc->data_details.ntc_144.new_cap_mask));
			break;
		case SM_SYS_IMG_GUID_CHANGED_TRAP:
			OSM_LOG(log, level,
				"Reporting Informational Notice \"%s\" from LID %u, "
				"GUID 0x%016" PRIx64 ", new SysImageGUID 0x%016" PRIx64 "\n",
				ib_get_trap_str(ntc->g_or_v.generic.trap_num),
				cl_ntoh16(ntc->issuer_lid),
				cl_ntoh64(gid->unicast.interface_id),
				cl_ntoh64(ntc->data_details.ntc_145.new_sys_guid));
			break;
		case SM_BAD_MKEY_TRAP:
			OSM_LOG(log, level,
				"Reporting Security Notice \"%s\" from LID %u, "
				"GUID 0x%016" PRIx64 ", Method 0x%x, Attribute 0x%x, "
				"AttrMod 0x%x, M_Key 0x%016" PRIx64 "\n",
				ib_get_trap_str(ntc->g_or_v.generic.trap_num),
				cl_ntoh16(ntc->issuer_lid),
				cl_ntoh64(gid->unicast.interface_id),
				ntc->data_details.ntc_256.method,
				cl_ntoh16(ntc->data_details.ntc_256.attr_id),
				cl_ntoh32(ntc->data_details.ntc_256.attr_mod),
				cl_ntoh64(ntc->data_details.ntc_256.mkey));
			break;
		case SM_BAD_PKEY_TRAP:
		case SM_BAD_QKEY_TRAP:
			gid1 = &ntc->data_details.ntc_257_258.gid1;
			gid2 = &ntc->data_details.ntc_257_258.gid2;
			OSM_LOG(log, level,
				"Reporting Security Notice \"%s\" from LID %u, "
				"GUID 0x%016" PRIx64 " : LID1 %u, LID2 %u, %s 0x%x, "
				"SL %d, QP1 0x%x, QP2 0x%x, GID1 %s, GID2 %s\n",
				ib_get_trap_str(ntc->g_or_v.generic.trap_num),
				cl_ntoh16(ntc->issuer_lid),
				cl_ntoh64(gid->unicast.interface_id),
				cl_ntoh16(ntc->data_details.ntc_257_258.lid1),
				cl_ntoh16(ntc->data_details.ntc_257_258.lid2),
				cl_ntoh16(ntc->g_or_v.generic.trap_num) == SM_BAD_QKEY_TRAP ?
					"Q_Key" : "P_Key",
				cl_ntoh32(ntc->data_details.ntc_257_258.key),
				cl_ntoh32(ntc->data_details.ntc_257_258.qp1) >> 28,
				cl_ntoh32(ntc->data_details.ntc_257_258.qp1) & 0xffffff,
				cl_ntoh32(ntc->data_details.ntc_257_258.qp2) & 0xffffff,
				inet_ntop(AF_INET6, gid1->raw, gid_str, sizeof gid_str),
				inet_ntop(AF_INET6, gid2->raw, gid_str2, sizeof gid_str2));
			break;
		case SM_BAD_SWITCH_PKEY_TRAP:
			gid1 = &ntc->data_details.ntc_259.gid1;
			gid2 = &ntc->data_details.ntc_259.gid2;
			OSM_LOG(log, level,
				"Reporting Security Notice \"%s\" from switch LID %u, "
				"GUID 0x%016" PRIx64 " port %d : data_valid 0x%04x, "
				"LID1 %u, LID2 %u, PKey 0x%04x, "
				"SL %d, QP1 0x%x, QP2 0x%x, GID1 %s, GID2 %s\n",
				ib_get_trap_str(ntc->g_or_v.generic.trap_num),
				cl_ntoh16(ntc->issuer_lid),
				cl_ntoh64(gid->unicast.interface_id),
				ntc->data_details.ntc_259.port_no,
				cl_ntoh16(ntc->data_details.ntc_259.data_valid),
				cl_ntoh16(ntc->data_details.ntc_259.lid1),
				cl_ntoh16(ntc->data_details.ntc_259.lid2),
				cl_ntoh16(ntc->data_details.ntc_257_258.key),
				cl_ntoh32(ntc->data_details.ntc_259.sl_qp1) >> 28,
				cl_ntoh32(ntc->data_details.ntc_259.sl_qp1) & 0xffffff,
				cl_ntoh32(ntc->data_details.ntc_259.qp2),
				inet_ntop(AF_INET6, gid1->raw, gid_str, sizeof gid_str),
				inet_ntop(AF_INET6, gid2->raw, gid_str2, sizeof gid_str2));
			break;
		default:
			OSM_LOG(log, level,
				"Reporting Generic Notice type:%u num:%u (%s)"
				" from LID:%u GID:%s\n",
				ib_notice_get_type(ntc),
				cl_ntoh16(ntc->g_or_v.generic.trap_num),
				ib_get_trap_str(ntc->g_or_v.generic.trap_num),
				cl_ntoh16(ntc->issuer_lid),
				inet_ntop(AF_INET6, gid->raw, gid_str, sizeof gid_str));
			break;
		}
	} else
		OSM_LOG(log, level,
			"Reporting Vendor Notice type:%u vend:%u dev:%u"
			" from LID:%u GID:%s\n",
			ib_notice_get_type(ntc),
			cl_ntoh32(ib_notice_get_vend_id(ntc)),
			cl_ntoh16(ntc->g_or_v.vend.dev_id),
			cl_ntoh16(ntc->issuer_lid),
			inet_ntop(AF_INET6, ntc->issuer_gid.raw, gid_str,
				  sizeof gid_str));
}