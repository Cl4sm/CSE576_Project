int osm_send_trap144(osm_sm_t * sm, ib_net16_t local)
{
	osm_madw_t *madw;
	ib_smp_t *smp;
	ib_mad_notice_attr_t *ntc;
	osm_port_t *port, *smport;
	ib_port_info_t *pi;

	port = osm_get_port_by_guid(sm->p_subn, sm->p_subn->sm_port_guid);
	if (!port) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR,
			"ERR 1104: cannot find SM port by guid 0x%" PRIx64 "\n",
			cl_ntoh64(sm->p_subn->sm_port_guid));
		return -1;
	}

	pi = &port->p_physp->port_info;

	/* don't bother with sending trap when SMA supports this */
	if (!local &&
	    pi->capability_mask&(IB_PORT_CAP_HAS_TRAP|IB_PORT_CAP_HAS_CAP_NTC))
		return 0;

	smport = osm_get_port_by_guid(sm->p_subn, sm->master_sm_guid);
	if (!smport) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR,
			"ERR 1106: cannot find master SM port by guid 0x%" PRIx64 "\n",
			cl_ntoh64(sm->master_sm_guid));
		return -1;
	}

	madw = osm_mad_pool_get(sm->p_mad_pool,
				osm_sm_mad_ctrl_get_bind_handle(&sm->mad_ctrl),
				MAD_BLOCK_SIZE, NULL);
	if (madw == NULL) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR,
			"ERR 1105: Unable to acquire MAD\n");
		return -1;
	}

	madw->mad_addr.dest_lid = smport->p_physp->port_info.base_lid;
	madw->mad_addr.addr_type.smi.source_lid = pi->base_lid;
	madw->resp_expected = TRUE;
	madw->fail_msg = CL_DISP_MSGID_NONE;

	smp = osm_madw_get_smp_ptr(madw);
	memset(smp, 0, sizeof(*smp));

	smp->base_ver = 1;
	smp->mgmt_class = IB_MCLASS_SUBN_LID;
	smp->class_ver = 1;
	smp->method = IB_MAD_METHOD_TRAP;
	smp->trans_id = cl_hton64((uint64_t) cl_atomic_inc(&sm->sm_trans_id)
							   & (uint64_t)(0xFFFFFFFF));
	if (smp->trans_id == 0)
		smp->trans_id = cl_hton64((uint64_t) cl_atomic_inc(&sm->sm_trans_id)
								   & (uint64_t)(0xFFFFFFFF));

	smp->attr_id = IB_MAD_ATTR_NOTICE;

	ntc = (ib_mad_notice_attr_t *) smp->data;

	ntc->generic_type = 0x80 | IB_NOTICE_TYPE_INFO;
	ib_notice_set_prod_type_ho(ntc, osm_node_get_type(port->p_node));
	ntc->g_or_v.generic.trap_num = cl_hton16(144);
	ntc->issuer_lid = pi->base_lid;
	ntc->data_details.ntc_144.lid = pi->base_lid;
	ntc->data_details.ntc_144.local_changes = local ?
	    TRAP_144_MASK_OTHER_LOCAL_CHANGES : 0;
	ntc->data_details.ntc_144.new_cap_mask = pi->capability_mask;
	ntc->data_details.ntc_144.change_flgs = local;

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Sending Trap 144, TID 0x%" PRIx64 " to SM lid %u\n",
		cl_ntoh64(smp->trans_id), cl_ntoh16(madw->mad_addr.dest_lid));

	osm_vl15_post(sm->p_vl15, madw);

	return 0;
}