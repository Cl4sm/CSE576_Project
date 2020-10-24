static int is_access_permitted(osm_infr_t *p_infr_rec,
			       osm_infr_match_ctxt_t *p_infr_match )
{
	cl_list_t *p_infr_to_remove_list = p_infr_match->p_remove_infr_list;
	ib_inform_info_t *p_ii = &(p_infr_rec->inform_record.inform_info);
	ib_mad_notice_attr_t *p_ntc = p_infr_match->p_ntc;
	uint16_t trap_num = cl_ntoh16(p_ntc->g_or_v.generic.trap_num);
	osm_subn_t *p_subn = p_infr_rec->sa->p_subn;
	osm_log_t *p_log = p_infr_rec->sa->p_log;
	osm_mgrp_t *p_mgrp;
	ib_gid_t source_gid;
	osm_port_t *p_src_port;
	osm_port_t *p_dest_port;

	/* In case of GID_IN(64) or GID_OUT(65) traps the source gid
	   comparison should be done on the trap source (saved as the gid in the
	   data details field).
	   For traps MC_CREATE(66) or MC_DELETE(67) the data details gid is
	   the MGID. We need to check whether the subscriber has a compatible
	   pkey with MC group.
	   In all other cases the issuer gid is the trap source.
	*/
	if (trap_num >= SM_GID_IN_SERVICE_TRAP &&
	    trap_num <= SM_MGID_DESTROYED_TRAP)
		/* The issuer of these traps is the SM so source_gid
		   is the gid saved on the data details */
		source_gid = p_ntc->data_details.ntc_64_67.gid;
	else
		source_gid = p_ntc->issuer_gid;

	p_dest_port = osm_get_port_by_lid(p_subn,
					  p_infr_rec->report_addr.dest_lid);
	if (!p_dest_port) {
		OSM_LOG(p_log, OSM_LOG_INFO,
			"Cannot find destination port with LID:%u\n",
			cl_ntoh16(p_infr_rec->report_addr.dest_lid));
		goto Exit;
	}

	/* Check if there is a pkey match. o13-17.1.1 */
	switch (trap_num) {
		case SM_MGID_CREATED_TRAP:
		case SM_MGID_DESTROYED_TRAP:
			p_mgrp = osm_get_mgrp_by_mgid(p_subn, &source_gid);
			if (!p_mgrp) {
				char gid_str[INET6_ADDRSTRLEN];
				OSM_LOG(p_log, OSM_LOG_INFO,
					"Cannot find MGID %s\n",
					inet_ntop(AF_INET6, source_gid.raw, gid_str, sizeof gid_str));
				goto Exit;
			}

			if (!osm_physp_has_pkey(p_log,
						p_mgrp->mcmember_rec.pkey,
						p_dest_port->p_physp)) {
				char gid_str[INET6_ADDRSTRLEN];
				OSM_LOG(p_log, OSM_LOG_INFO,
					"MGID %s and port GUID:0x%016" PRIx64 " do not share same pkey\n",
					inet_ntop(AF_INET6, source_gid.raw, gid_str, sizeof gid_str),
					cl_ntoh64(p_dest_port->guid));
				goto Exit;
			}
			break;

		default:
			p_src_port =
			    osm_get_port_by_guid(p_subn, source_gid.unicast.interface_id);
			if (!p_src_port) {
				OSM_LOG(p_log, OSM_LOG_INFO,
					"Cannot find source port with GUID:0x%016" PRIx64 "\n",
					cl_ntoh64(source_gid.unicast.interface_id));
				goto Exit;
			}


			if (osm_port_share_pkey(p_log, p_src_port, p_dest_port,
						p_subn->opt.allow_both_pkeys) == FALSE) {
				OSM_LOG(p_log, OSM_LOG_DEBUG, "Mismatch by Pkey\n");
				/* According to o13-17.1.2 - If this informInfo
				   does not have lid_range_begin of 0xFFFF,
				   then this informInfo request should be
				   removed from database */
				if (p_ii->lid_range_begin != 0xFFFF) {
					OSM_LOG(p_log, OSM_LOG_VERBOSE,
						"Pkey mismatch on lid_range_begin != 0xFFFF. "
						"Need to remove this informInfo from db\n");
					/* add the informInfo record to the remove_infr list */
					cl_list_insert_tail(p_infr_to_remove_list, p_infr_rec);
				}
				goto Exit;
			}
			break;
	}

	return 1;
Exit:
	return 0;
}