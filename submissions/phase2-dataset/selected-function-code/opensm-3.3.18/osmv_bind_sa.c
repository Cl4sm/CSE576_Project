osm_bind_handle_t
osmv_bind_sa(IN osm_vendor_t * const p_vend,
	     IN osm_mad_pool_t * const p_mad_pool, IN ib_net64_t port_guid)
{
	osm_bind_info_t bind_info;
	osm_log_t *p_log = p_vend->p_log;
	ib_api_status_t status = IB_SUCCESS;
	osmv_sa_bind_info_t *p_sa_bind_info;
	cl_status_t cl_status;

	OSM_LOG_ENTER(p_log);

	OSM_LOG(p_log, OSM_LOG_DEBUG,
		"Binding to port 0x%" PRIx64 "\n", cl_ntoh64(port_guid));

	bind_info.port_guid = port_guid;
	bind_info.mad_class = IB_MCLASS_SUBN_ADM;
	bind_info.class_version = 2;
	bind_info.is_responder = FALSE;
	bind_info.is_trap_processor = FALSE;
	bind_info.is_report_processor = FALSE;
	bind_info.send_q_size = 256;
	bind_info.recv_q_size = 256;

	/* allocate the new sa bind info */
	p_sa_bind_info =
	    (osmv_sa_bind_info_t *) malloc(sizeof(osmv_sa_bind_info_t));
	if (!p_sa_bind_info) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0505: "
			"Failed to allocate new bind structure\n");
		p_sa_bind_info = OSM_BIND_INVALID_HANDLE;
		goto Exit;
	}

	/* store some important context */
	p_sa_bind_info->p_log = p_log;
	p_sa_bind_info->port_guid = port_guid;
	p_sa_bind_info->p_mad_pool = p_mad_pool;
	p_sa_bind_info->p_vendor = p_vend;
	p_sa_bind_info->last_lids_update_sec = 0;

	/* Bind to the lower level */
	p_sa_bind_info->h_bind = osm_vendor_bind(p_vend, &bind_info, p_mad_pool, __osmv_sa_mad_rcv_cb, __osmv_sa_mad_err_cb, p_sa_bind_info);	/* context provided to CBs */

	if (p_sa_bind_info->h_bind == OSM_BIND_INVALID_HANDLE) {
		free(p_sa_bind_info);
		p_sa_bind_info = OSM_BIND_INVALID_HANDLE;
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0506: "
			"Failed to bind to vendor GSI\n");
		goto Exit;
	}

	/* obtain the sm_lid from the vendor */
	status =
	    __osmv_get_lid_and_sm_lid_by_port_guid(p_vend, port_guid,
						   &p_sa_bind_info->
						   last_lids_update_sec,
						   &p_sa_bind_info->lid,
						   &p_sa_bind_info->sm_lid);
	if (status != IB_SUCCESS) {
		free(p_sa_bind_info);
		p_sa_bind_info = OSM_BIND_INVALID_HANDLE;
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0507: "
			"Failed to obtain the SM lid\n");
		goto Exit;
	}

	/* initialize the sync_event */
	cl_event_construct(&p_sa_bind_info->sync_event);
	cl_status = cl_event_init(&p_sa_bind_info->sync_event, TRUE);
	if (cl_status != CL_SUCCESS) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0508: "
			"cl_init_event failed: %s\n", ib_get_err_str(cl_status));
		free(p_sa_bind_info);
		p_sa_bind_info = OSM_BIND_INVALID_HANDLE;
	}

Exit:
	OSM_LOG_EXIT(p_log);
	return (p_sa_bind_info);
}