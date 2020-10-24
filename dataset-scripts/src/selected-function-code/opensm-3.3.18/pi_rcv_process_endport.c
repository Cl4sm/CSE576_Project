static void pi_rcv_process_endport(IN osm_sm_t * sm, IN osm_physp_t * p_physp,
				   IN const ib_port_info_t * p_pi)
{
	osm_madw_context_t context;
	ib_api_status_t status;
	ib_net64_t port_guid;
	int extended;
	uint8_t rate, mtu, mpb;
	unsigned data_vls;
	cl_qmap_t *p_sm_tbl;
	osm_remote_sm_t *p_sm;

	OSM_LOG_ENTER(sm->p_log);

	port_guid = osm_physp_get_port_guid(p_physp);

	/* HACK extended port 0 should be handled too! */
	if (osm_physp_get_port_num(p_physp) != 0 &&
	    ib_port_info_get_port_state(p_pi) != IB_LINK_DOWN) {
		/* track the minimal endport MTU, rate, and operational VLs */
		mtu = ib_port_info_get_mtu_cap(p_pi);
		if (mtu < sm->p_subn->min_ca_mtu) {
			OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
				"Setting endport minimal MTU to:%u defined by port:0x%"
				PRIx64 "\n", mtu, cl_ntoh64(port_guid));
			sm->p_subn->min_ca_mtu = mtu;
		}

		extended = p_pi->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS;
		rate = ib_port_info_compute_rate(p_pi, extended);
		if (rate < sm->p_subn->min_ca_rate) {
			OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
				"Setting endport minimal rate to:%u defined by port:0x%"
				PRIx64 "\n", rate, cl_ntoh64(port_guid));
			sm->p_subn->min_ca_rate = rate;
		}

		data_vls = 1U << (ib_port_info_get_vl_cap(p_pi) - 1);
		if (data_vls > sm->p_subn->opt.max_op_vls)
			data_vls = sm->p_subn->opt.max_op_vls;
		if (data_vls >= IB_MAX_NUM_VLS)
			data_vls = IB_MAX_NUM_VLS - 1;
		if ((uint8_t)data_vls < sm->p_subn->min_data_vls) {
			OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
				"Setting endport minimal data VLs to:%u defined by port:0x%"
				PRIx64 "\n", data_vls, cl_ntoh64(port_guid));
			sm->p_subn->min_data_vls = data_vls;
		}
	}

	/* Check M_Key vs M_Key protect, can we control the port ? */
	mpb = ib_port_info_get_mpb(p_pi);
	if (mpb > 0 && p_pi->m_key == 0) {
		OSM_LOG(sm->p_log, OSM_LOG_INFO,
			"Port 0x%" PRIx64 " has unknown M_Key, protection level %u\n",
			cl_ntoh64(port_guid), mpb);
	}

	if (port_guid != sm->p_subn->sm_port_guid) {
		p_sm_tbl = &sm->p_subn->sm_guid_tbl;
		if (p_pi->capability_mask & IB_PORT_CAP_IS_SM) {
			/*
			 * Before querying the SM - we want to make sure we
			 * clean its state, so if the querying fails we
			 * recognize that this SM is not active.
			 */
			p_sm =
			    (osm_remote_sm_t *) cl_qmap_get(p_sm_tbl,
							    port_guid);
			if (p_sm != (osm_remote_sm_t *) cl_qmap_end(p_sm_tbl))
				/* clean it up */
				p_sm->smi.pri_state =
				    0xF0 & p_sm->smi.pri_state;
			if (sm->p_subn->opt.ignore_other_sm)
				OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
					"Ignoring SM on port 0x%" PRIx64 "\n",
					cl_ntoh64(port_guid));
			else {
				OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
					"Detected another SM. Requesting SMInfo "
					"from port 0x%" PRIx64 "\n",
					cl_ntoh64(port_guid));

				/*
				   This port indicates it's an SM and
				   it's not our own port.
				   Acquire the SMInfo Attribute.
				 */
				memset(&context, 0, sizeof(context));
				context.smi_context.set_method = FALSE;
				context.smi_context.port_guid = port_guid;
				status = osm_req_get(sm,
						     osm_physp_get_dr_path_ptr
						     (p_physp),
						     IB_MAD_ATTR_SM_INFO, 0,
						     FALSE,
						     ib_port_info_get_m_key(&p_physp->port_info),
						     CL_DISP_MSGID_NONE,
						     &context);

				if (status != IB_SUCCESS)
					OSM_LOG(sm->p_log, OSM_LOG_ERROR,
						"ERR 0F05: "
						"Failure requesting SMInfo (%s) "
						"from port 0x%" PRIx64 "\n",
						ib_get_err_str(status),
						cl_ntoh64(port_guid));
			}
		} else {
			p_sm =
			    (osm_remote_sm_t *) cl_qmap_remove(p_sm_tbl,
							       port_guid);
			if (p_sm != (osm_remote_sm_t *) cl_qmap_end(p_sm_tbl))
				free(p_sm);
		}
	}

	OSM_LOG_EXIT(sm->p_log);
}