void osm_smir_rcv_process(IN void *ctx, IN void *data)
{
	osm_sa_t *sa = ctx;
	osm_madw_t *p_madw = data;
	const ib_sa_mad_t *sad_mad;
	const ib_sminfo_record_t *p_rcvd_rec;
	const osm_port_t *p_port = NULL;
	const ib_sm_info_t *p_smi;
	cl_qlist_t rec_list;
	osm_smir_search_ctxt_t context;
	ib_api_status_t status = IB_SUCCESS;
	ib_net64_t comp_mask;
	ib_net64_t port_guid;
	osm_physp_t *p_req_physp;
	osm_port_t *local_port;
	osm_remote_sm_t *p_rem_sm;
	cl_qmap_t *p_sm_guid_tbl;
	uint8_t pri_state;

	CL_ASSERT(sa);

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	sad_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_rcvd_rec = (ib_sminfo_record_t *) ib_sa_mad_get_payload_ptr(sad_mad);
	comp_mask = sad_mad->comp_mask;

	CL_ASSERT(sad_mad->attr_id == IB_MAD_ATTR_SMINFO_RECORD);

	/* we only support SubnAdmGet and SubnAdmGetTable methods */
	if (sad_mad->method != IB_MAD_METHOD_GET &&
	    sad_mad->method != IB_MAD_METHOD_GETTABLE) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2804: "
			"Unsupported Method (%s) for SMInfoRecord request\n",
			ib_get_sa_method_str(sad_mad->method));
		osm_sa_send_error(sa, p_madw, IB_MAD_STATUS_UNSUP_METHOD_ATTR);
		goto Exit;
	}

	cl_plock_acquire(sa->p_lock);

	/* update the requester physical port */
	p_req_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
						osm_madw_get_mad_addr_ptr
						(p_madw));
	if (p_req_physp == NULL) {
		cl_plock_release(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2803: "
			"Cannot find requester physical port\n");
		goto Exit;
	}

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requester port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));
		osm_dump_sm_info_record_v2(sa->p_log, p_rcvd_rec, FILE_ID, OSM_LOG_DEBUG);
	}

	p_smi = &p_rcvd_rec->sm_info;

	cl_qlist_init(&rec_list);

	context.p_rcvd_rec = p_rcvd_rec;
	context.p_list = &rec_list;
	context.comp_mask = sad_mad->comp_mask;
	context.sa = sa;
	context.p_req_physp = p_req_physp;

	/*
	   If the user specified a LID, it obviously narrows our
	   work load, since we don't have to search every port
	 */
	if (comp_mask & IB_SMIR_COMPMASK_LID) {
		p_port = osm_get_port_by_lid(sa->p_subn, p_rcvd_rec->lid);
		if (!p_port) {
			status = IB_NOT_FOUND;
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2806: "
				"No port found with LID %u\n",
				cl_ntoh16(p_rcvd_rec->lid));
		}
	}

	if (status == IB_SUCCESS) {
		/* Handle our own SM first */
		local_port = osm_get_port_by_guid(sa->p_subn,
						  sa->p_subn->sm_port_guid);
		if (!local_port) {
			cl_plock_release(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2809: "
				"No port found with GUID 0x%016" PRIx64 "\n",
				cl_ntoh64(sa->p_subn->sm_port_guid));
			goto Exit;
		}

		if (!p_port || local_port == p_port) {
			if (FALSE ==
			    osm_physp_share_pkey(sa->p_log, p_req_physp,
						 local_port->p_physp,
						 sa->p_subn->opt.allow_both_pkeys)) {
				cl_plock_release(sa->p_lock);
				OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2805: "
					"Cannot get SMInfo record due to pkey violation\n");
				goto Exit;
			}

			/* Check that other search components specified match */
			if ((comp_mask & IB_SMIR_COMPMASK_GUID) &&
			    sa->p_subn->sm_port_guid != p_smi->guid)
				goto Remotes;
			if ((comp_mask & IB_SMIR_COMPMASK_PRIORITY) &&
			    sa->p_subn->opt.sm_priority !=
			    ib_sminfo_get_priority(p_smi))
				goto Remotes;
			if ((comp_mask & IB_SMIR_COMPMASK_SMSTATE) &&
			    sa->p_subn->sm_state != ib_sminfo_get_state(p_smi))
				goto Remotes;

			/* Now, add local SMInfo to list */
			pri_state = sa->p_subn->sm_state & 0x0F;
			pri_state |= (sa->p_subn->opt.sm_priority & 0x0F) << 4;
			smir_rcv_new_smir(sa, local_port, context.p_list,
					  sa->p_subn->sm_port_guid,
					  cl_ntoh32(sa->p_subn->p_osm->stats.
						    qp0_mads_sent), pri_state,
					  p_req_physp);
		}

	      Remotes:
		if (p_port && p_port != local_port) {
			/* Find remote SM corresponding to p_port */
			port_guid = osm_port_get_guid(p_port);
			p_sm_guid_tbl = &sa->p_subn->sm_guid_tbl;
			p_rem_sm =
			    (osm_remote_sm_t *) cl_qmap_get(p_sm_guid_tbl,
							    port_guid);
			if (p_rem_sm !=
			    (osm_remote_sm_t *) cl_qmap_end(p_sm_guid_tbl))
				sa_smir_by_comp_mask(sa, p_rem_sm, &context);
			else
				OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 280A: "
					"No remote SM for GUID 0x%016" PRIx64
					"\n", cl_ntoh64(port_guid));
		} else if (!p_port) {
			/* Go over all other known (remote) SMs */
			cl_qmap_apply_func(&sa->p_subn->sm_guid_tbl,
					   sa_smir_by_comp_mask_cb, &context);
		}
	}

	cl_plock_release(sa->p_lock);

	osm_sa_respond(sa, p_madw, sizeof(ib_sminfo_record_t), &rec_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}