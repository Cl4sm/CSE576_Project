ib_api_status_t
osmv_transport_init(IN osm_bind_info_t * p_info,
		    IN char hca_id[VENDOR_HCA_MAXNAMES],
		    IN uint8_t hca_idx, IN osmv_bind_obj_t * p_bo)
{
	ibms_conn_handle_t conHdl;	/* the connection we talk to the simulator through */
	osmv_ibms_transport_mgr_t *p_mgr =
	    malloc(sizeof(osmv_ibms_transport_mgr_t));
	int qpn;
	int ibms_status;
	uint64_t port_guid;

	if (!p_mgr) {
		return IB_INSUFFICIENT_MEMORY;
	}

	memset(p_mgr, 0, sizeof(osmv_ibms_transport_mgr_t));

	/* create the client socket connected to the simulator */
	/* also perform the "connect" message - such that we
	   validate the target guid */
	if (osm_vendor_get_guid_by_ca_and_port
	    (p_bo->p_vendor, hca_id, p_bo->port_num, &port_guid)) {
		return IB_INVALID_GUID;
	}

	conHdl =
	    ibms_connect(port_guid, __osmv_ibms_receiver_callback,
			 (void *)p_bo);
	if (!conHdl) {
		printf("fail to connect to the server.\n");
		exit(1);
	}

	/*
	 * Create the MAD filter on this file handle.
	 */

	p_mgr->filter.port = p_bo->port_num;
	p_mgr->filter.only_input = 1;
	p_mgr->filter.mask =
	    IBMS_BIND_MASK_PORT |
	    IBMS_BIND_MASK_INPUT | IBMS_BIND_MASK_QP | IBMS_BIND_MASK_CLASS;

	switch (p_info->mad_class) {
	case IB_MCLASS_SUBN_LID:
	case IB_MCLASS_SUBN_DIR:
		qpn = 0;
		p_mgr->filter.qpn = qpn;
		p_mgr->filter.mgt_class = IB_MCLASS_SUBN_LID;
		ibms_status = ibms_bind(conHdl, &p_mgr->filter);
		if (ibms_status) {
			return IB_ERROR;
		}

		p_mgr->filter.mgt_class = IB_MCLASS_SUBN_DIR;
		ibms_status = ibms_bind(conHdl, &p_mgr->filter);
		if (ibms_status) {
			return IB_ERROR;
		}

		break;

	case IB_MCLASS_SUBN_ADM:
	default:
		qpn = 1;
		p_mgr->filter.qpn = qpn;
		p_mgr->filter.mgt_class = p_info->mad_class;
		ibms_status = ibms_bind(conHdl, &p_mgr->filter);
		if (ibms_status) {
			return IB_ERROR;
		}
		break;
	}

	p_mgr->conHdl = conHdl;

	p_bo->p_transp_mgr = p_mgr;

	/* Initialize the magic_ptr to the pointer of the p_bo info.
	   This will be used to signal when the object is being destroyed, so no
	   real action will be done then. */
	p_bo->magic_ptr = p_bo;

	return IB_SUCCESS;
}