static void match_notice_to_inf_rec(IN cl_list_item_t * p_list_item,
				    IN void *context)
{
	osm_infr_match_ctxt_t *p_infr_match = (osm_infr_match_ctxt_t *) context;
	ib_mad_notice_attr_t *p_ntc = p_infr_match->p_ntc;
	osm_infr_t *p_infr_rec = (osm_infr_t *) p_list_item;
	ib_inform_info_t *p_ii = &(p_infr_rec->inform_record.inform_info);
	osm_log_t *p_log = p_infr_rec->sa->p_log;

	OSM_LOG_ENTER(p_log);

	/* matching rules
	 * InformInfo   Notice
	 * GID          IssuerGID    if non zero must match the trap
	 * LIDRange     IssuerLID    apply only if GID=0
	 * IsGeneric    IsGeneric    is compulsory and must match the trap
	 * Type         Type         if not 0xFFFF must match
	 * TrapNumber   TrapNumber   if not 0xFFFF must match
	 * DeviceId     DeviceID     if not 0xFFFF must match
	 * QPN dont care
	 * ProducerType ProducerType match or 0xFFFFFF // EZ: actually my interpretation
	 * VendorID     VendorID     match or 0xFFFFFF
	 */

	/* GID          IssuerGID    if non zero must match the trap  */
	if (p_ii->gid.unicast.prefix != 0
	    || p_ii->gid.unicast.interface_id != 0) {
		/* match by GID */
		if (memcmp(&(p_ii->gid), &(p_ntc->issuer_gid),
			   sizeof(ib_gid_t))) {
			OSM_LOG(p_log, OSM_LOG_DEBUG, "Mismatch by GID\n");
			goto Exit;
		}
	} else {
		/* LIDRange     IssuerLID    apply only if GID=0 */
		/* If lid_range_begin of the informInfo is 0xFFFF - then it should be ignored. */
		if (p_ii->lid_range_begin != 0xFFFF) {
			/* a real lid range is given - check it */
			if ((cl_hton16(p_ii->lid_range_begin) >
			     cl_hton16(p_ntc->issuer_lid))
			    || (cl_hton16(p_ntc->issuer_lid) >
				cl_hton16(p_ii->lid_range_end))) {
				OSM_LOG(p_log, OSM_LOG_DEBUG,
					"Mismatch by LID Range. Needed: %u <= %u <= %u\n",
					cl_hton16(p_ii->lid_range_begin),
					cl_hton16(p_ntc->issuer_lid),
					cl_hton16(p_ii->lid_range_end));
				goto Exit;
			}
		}
	}

	/* IsGeneric    IsGeneric    is compulsory and must match the trap  */
	if ((p_ii->is_generic && !ib_notice_is_generic(p_ntc)) ||
	    (!p_ii->is_generic && ib_notice_is_generic(p_ntc))) {
		OSM_LOG(p_log, OSM_LOG_DEBUG, "Mismatch by Generic/Vendor\n");
		goto Exit;
	}

	/* Type         Type         if not 0xFFFF must match */
	if ((p_ii->trap_type != 0xFFFF) &&
	    (cl_ntoh16(p_ii->trap_type) != ib_notice_get_type(p_ntc))) {
		OSM_LOG(p_log, OSM_LOG_DEBUG, "Mismatch by Type\n");
		goto Exit;
	}

	/* based on generic type */
	if (p_ii->is_generic) {
		/* TrapNumber   TrapNumber   if not 0xFFFF must match */
		if ((p_ii->g_or_v.generic.trap_num != 0xFFFF) &&
		    (p_ii->g_or_v.generic.trap_num !=
		     p_ntc->g_or_v.generic.trap_num)) {
			OSM_LOG(p_log, OSM_LOG_DEBUG, "Mismatch by Trap Num\n");
			goto Exit;
		}

		/* ProducerType ProducerType match or 0xFFFFFF  */
		if ((cl_ntoh32(ib_inform_info_get_prod_type(p_ii)) != 0xFFFFFF)
		    && (ib_inform_info_get_prod_type(p_ii) !=
			ib_notice_get_prod_type(p_ntc))) {
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Mismatch by Node Type: II=0x%06X (%s) Trap=0x%06X (%s)\n",
				cl_ntoh32(ib_inform_info_get_prod_type(p_ii)),
				ib_get_producer_type_str
				(ib_inform_info_get_prod_type(p_ii)),
				cl_ntoh32(ib_notice_get_prod_type(p_ntc)),
				ib_get_producer_type_str(ib_notice_get_prod_type
							 (p_ntc)));
			goto Exit;
		}
	} else {
		/* DeviceId     DeviceID     if not 0xFFFF must match */
		if ((p_ii->g_or_v.vend.dev_id != 0xFFFF) &&
		    (p_ii->g_or_v.vend.dev_id != p_ntc->g_or_v.vend.dev_id)) {
			OSM_LOG(p_log, OSM_LOG_DEBUG, "Mismatch by Dev Id\n");
			goto Exit;
		}

		/* VendorID     VendorID     match or 0xFFFFFF  */
		if ((ib_inform_info_get_vend_id(p_ii) != CL_HTON32(0xFFFFFF)) &&
		    (ib_inform_info_get_vend_id(p_ii) !=
		     ib_notice_get_vend_id(p_ntc))) {
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Mismatch by Vendor ID\n");
			goto Exit;
		}
	}

	if (!is_access_permitted(p_infr_rec, p_infr_match))
		goto Exit;

	/* send the report to the address provided in the inform record */
	OSM_LOG(p_log, OSM_LOG_DEBUG, "MATCH! Sending Report...\n");
	send_report(p_infr_rec, p_ntc);

Exit:
	OSM_LOG_EXIT(p_log);
}