static boolean_t validate_ports_access_rights(IN osm_sa_t * sa,
					      IN osm_infr_t * p_infr_rec)
{
	boolean_t valid = TRUE;
	osm_physp_t *p_requester_physp;
	osm_port_t *p_port;
	ib_net64_t portguid;
	uint16_t lid_range_begin, lid_range_end, lid;

	OSM_LOG_ENTER(sa->p_log);

	/* get the requester physp from the request address */
	p_requester_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
						      &p_infr_rec->report_addr);

	if (ib_gid_is_notzero(&p_infr_rec->inform_record.inform_info.gid)) {
		/* a gid is defined */
		portguid =
		    p_infr_rec->inform_record.inform_info.gid.unicast.
		    interface_id;

		p_port = osm_get_port_by_guid(sa->p_subn, portguid);
		if (p_port == NULL) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4301: "
				"Invalid port guid: 0x%016" PRIx64 "\n",
				cl_ntoh64(portguid));
			valid = FALSE;
			goto Exit;
		}

		/* make sure that the requester and destination port can access
		   each other according to the current partitioning. */
		if (!osm_physp_share_pkey
		    (sa->p_log, p_port->p_physp, p_requester_physp,
		     sa->p_subn->opt.allow_both_pkeys)) {
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"port and requester don't share pkey\n");
			valid = FALSE;
			goto Exit;
		}
	} else {
		size_t lids_size;

		/* gid is zero - check if LID range is defined */
		lid_range_begin =
		    cl_ntoh16(p_infr_rec->inform_record.inform_info.
			      lid_range_begin);
		/* if lid is 0xFFFF - meaning all endports managed by the manager */
		if (lid_range_begin == 0xFFFF)
			goto Exit;

		lid_range_end =
		    cl_ntoh16(p_infr_rec->inform_record.inform_info.
			      lid_range_end);

		lids_size = cl_ptr_vector_get_size(&sa->p_subn->port_lid_tbl);

		/* lid_range_end is set to zero if no range desired. In this
		   case - just make it equal to the lid_range_begin. */
		if (lid_range_end == 0)
			lid_range_end = lid_range_begin;
		else if (lid_range_end >= lids_size)
			lid_range_end = lids_size - 1;

		if (lid_range_begin >= lids_size) {
			/* requested lids are out of range */
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4302: "
				"Given LIDs (%u-%u) are out of range (%zu)\n",
				lid_range_begin, lid_range_end, lids_size);
			valid = FALSE;
			goto Exit;
		}

		/* go over all defined lids within the range and make sure that the
		   requester port can access them according to current partitioning. */
		for (lid = lid_range_begin; lid <= lid_range_end; lid++) {
			p_port = osm_get_port_by_lid_ho(sa->p_subn, lid);
			if (p_port == NULL)
				continue;

			/* make sure that the requester and destination port can access
			   each other according to the current partitioning. */
			if (!osm_physp_share_pkey
			    (sa->p_log, p_port->p_physp, p_requester_physp,
			     sa->p_subn->opt.allow_both_pkeys)) {
				OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
					"port and requester don't share pkey\n");
				valid = FALSE;
				goto Exit;
			}
		}
	}

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return valid;
}