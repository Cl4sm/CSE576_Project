static void infr_rcv_process_set_method(osm_sa_t * sa, IN osm_madw_t * p_madw)
{
	ib_sa_mad_t *p_sa_mad;
	ib_inform_info_t *p_recvd_inform_info;
	osm_infr_t inform_info_rec;	/* actual inform record to be stored for reports */
	osm_infr_t *p_infr;
	ib_net32_t qpn;
	uint8_t resp_time_val;
	ib_api_status_t res;

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_recvd_inform_info =
	    (ib_inform_info_t *) ib_sa_mad_get_payload_ptr(p_sa_mad);

#if 0
	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG))
		osm_dump_inform_info_v2(sa->p_log, p_recvd_inform_info,
				        FILE_ID, OSM_LOG_DEBUG);
#endif

	/* Grab the lock */
	cl_plock_excl_acquire(sa->p_lock);

	/* define the inform record */
	inform_info_rec.inform_record.inform_info = *p_recvd_inform_info;

	/* following C13-32.1.2 Tbl 120: we only copy the source address vector */
	inform_info_rec.report_addr = p_madw->mad_addr;

	/* we will need to know the mad srvc to send back through */
	inform_info_rec.h_bind = p_madw->h_bind;
	inform_info_rec.sa = sa;

	/* update the subscriber GID according to mad address */
	res = osm_get_gid_by_mad_addr(sa->p_log, sa->p_subn, &p_madw->mad_addr,
				      &inform_info_rec.inform_record.
				      subscriber_gid);
	if (res != IB_SUCCESS) {
		cl_plock_release(sa->p_lock);

		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4308: "
			"Subscribe Request from unknown LID: %u\n",
			cl_ntoh16(p_madw->mad_addr.dest_lid));
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	/* HACK: enum is always 0 (currently) */
	inform_info_rec.inform_record.subscriber_enum = 0;

	/* Subscribe values above 1 are undefined */
	if (p_recvd_inform_info->subscribe > 1) {
		cl_plock_release(sa->p_lock);

		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 430A: "
			"Invalid subscribe: %d\n",
			p_recvd_inform_info->subscribe);
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	/*
	 * Per C15-0.2-1.16, SubnAdmSet(InformInfo) subscriptions for
	 * SM security traps shall be provided only if they come from a
	 * trusted source.
	 */
	if ((p_sa_mad->sm_key == 0) && p_recvd_inform_info->is_generic &&
	    ((cl_ntoh16(p_recvd_inform_info->g_or_v.generic.trap_num) >= 256) &&
	     (cl_ntoh16(p_recvd_inform_info->g_or_v.generic.trap_num) <= 259))) {
		cl_plock_release(sa->p_lock);

		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 430B "
			"Request for security trap from non-trusted requester: "
			"Given SM_Key:0x%016" PRIx64 "\n",
			cl_ntoh64(p_sa_mad->sm_key));
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	/*
	 * MODIFICATIONS DONE ON INCOMING REQUEST:
	 *
	 * QPN:
	 * Internally we keep the QPN field of the InformInfo updated
	 * so we can simply compare it in the record - when finding such.
	 */
	if (p_recvd_inform_info->subscribe) {
		ib_inform_info_set_qpn(&inform_info_rec.inform_record.
				       inform_info,
				       inform_info_rec.report_addr.addr_type.
				       gsi.remote_qp);

		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Subscribe Request with QPN: 0x%06X\n",
			cl_ntoh32(inform_info_rec.report_addr.addr_type.gsi.
				  remote_qp));
	} else {
		ib_inform_info_get_qpn_resp_time(p_recvd_inform_info->g_or_v.
						 generic.qpn_resp_time_val,
						 &qpn, &resp_time_val);

		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"UnSubscribe Request with QPN: 0x%06X\n",
			cl_ntoh32(qpn));
	}

	/* If record exists with matching InformInfo */
	p_infr = osm_infr_get_by_rec(sa->p_subn, sa->p_log, &inform_info_rec);

	/* check to see if the request was for subscribe */
	if (p_recvd_inform_info->subscribe) {
		/* validate the request for a new or update InformInfo */
		if (validate_infr(sa, &inform_info_rec) != TRUE) {
			cl_plock_release(sa->p_lock);

			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4305: "
				"Failed to validate a new inform object\n");

			/* o13-13.1.1: we need to set the subscribe bit to 0 */
			p_recvd_inform_info->subscribe = 0;
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			goto Exit;
		}

		/* ok - we can try and create a new entry */
		if (p_infr == NULL) {
			/* Create the instance of the osm_infr_t object */
			p_infr = osm_infr_new(&inform_info_rec);
			if (p_infr == NULL) {
				cl_plock_release(sa->p_lock);

				OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4306: "
					"Failed to create a new inform object\n");

				/* o13-13.1.1: we need to set the subscribe bit to 0 */
				p_recvd_inform_info->subscribe = 0;
				osm_sa_send_error(sa, p_madw,
						  IB_SA_MAD_STATUS_NO_RESOURCES);
				goto Exit;
			}

			OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
				"Adding event subscription for port 0x%" PRIx64 "\n",
				cl_ntoh64(inform_info_rec.inform_record.subscriber_gid.unicast.interface_id));

			/* Add this new osm_infr_t object to subnet object */
			osm_infr_insert_to_db(sa->p_subn, sa->p_log, p_infr);
		} else
			/* Update the old instance of the osm_infr_t object */
			p_infr->inform_record = inform_info_rec.inform_record;
		/* We got an UnSubscribe request */
	} else if (p_infr == NULL) {
		cl_plock_release(sa->p_lock);

		/* No Such Item - So Error */
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4307: "
			"Failed to UnSubscribe to non existing inform object\n");

		/* o13-13.1.1: we need to set the subscribe bit to 0 */
		p_recvd_inform_info->subscribe = 0;
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	} else {
		/* Delete this object from the subnet list of informs */
		OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
			"Removing event subscription for port 0x%" PRIx64 "\n",
			cl_ntoh64(inform_info_rec.inform_record.subscriber_gid.unicast.interface_id));
		osm_infr_remove_from_db(sa->p_subn, sa->p_log, p_infr);
	}

	cl_plock_release(sa->p_lock);

	/* send the success response */
	infr_rcv_respond(sa, p_madw);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}