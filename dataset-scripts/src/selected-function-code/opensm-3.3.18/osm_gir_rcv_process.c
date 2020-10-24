void osm_gir_rcv_process(IN void *ctx, IN void *data)
{
	osm_sa_t *sa = ctx;
	osm_madw_t *p_madw = data;
	const ib_sa_mad_t *p_rcvd_mad;
	osm_physp_t *p_req_physp;
	osm_port_t *p_port;
	const ib_guidinfo_record_t *p_rcvd_rec;

	CL_ASSERT(sa);

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_rcvd_mad = osm_madw_get_sa_mad_ptr(p_madw);

	CL_ASSERT(p_rcvd_mad->attr_id == IB_MAD_ATTR_GUIDINFO_RECORD);

	switch(p_rcvd_mad->method) {
	case IB_MAD_METHOD_GET:
	case IB_MAD_METHOD_GETTABLE:
		/* update the requester physical port */
		CL_PLOCK_ACQUIRE(sa->p_lock);
		p_req_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
							osm_madw_get_mad_addr_ptr(p_madw));
		if (p_req_physp == NULL) {
			CL_PLOCK_RELEASE(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5104: "
				"Cannot find requester physical port\n");
			goto Exit;
		}
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requester port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));

		get_guidinfo(sa, p_madw, p_req_physp);
		goto Exit;
	case IB_MAD_METHOD_SET:
	case IB_MAD_METHOD_DELETE:
		if (!check_mod_comp_mask(p_rcvd_mad->comp_mask)) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5106: "
				"component mask = 0x%016" PRIx64 ", "
				"expected comp mask = 0x%016" PRIx64 "\n",
				cl_ntoh64(p_rcvd_mad->comp_mask),
				CL_NTOH64(MOD_GIR_COMP_MASK));
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_INSUF_COMPS);
			goto Exit;
		}
		p_rcvd_rec = (ib_guidinfo_record_t *) ib_sa_mad_get_payload_ptr(p_rcvd_mad);

		/* update the requester physical port */
		CL_PLOCK_EXCL_ACQUIRE(sa->p_lock);
		p_req_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
							osm_madw_get_mad_addr_ptr(p_madw));
		if (p_req_physp == NULL) {
			CL_PLOCK_RELEASE(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5104: "
				"Cannot find requester physical port\n");
			goto Exit;
		}
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requester port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));

		p_port = osm_get_port_by_lid(sa->p_subn, p_rcvd_rec->lid);
		if (!p_port) {
			CL_PLOCK_RELEASE(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5117: "
				"Port with LID %u not found\n",
				cl_ntoh16(p_rcvd_rec->lid));
				osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_NO_RECORDS);
			goto Exit;
		}
		if (!osm_physp_share_pkey(sa->p_log, p_req_physp, p_port->p_physp,
					  sa->p_subn->opt.allow_both_pkeys)) {
			CL_PLOCK_RELEASE(sa->p_lock);
			goto Exit;
		}

		if (p_rcvd_mad->method == IB_MAD_METHOD_SET)
			set_guidinfo(sa, p_madw, p_port, p_rcvd_rec->block_num);
		else
			del_guidinfo(sa, p_madw, p_port, p_rcvd_rec->block_num);
		break;
	default:
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5105: "
			"Unsupported Method (%s) for GUIDInfoRecord request\n",
			ib_get_sa_method_str(p_rcvd_mad->method));
		osm_sa_send_error(sa, p_madw, IB_MAD_STATUS_UNSUP_METHOD_ATTR);
		break;
	}

Exit:
	OSM_LOG_EXIT(sa->p_log);
}