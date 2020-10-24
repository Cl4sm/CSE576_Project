static void resp_make_resp_smp(IN osm_sm_t * sm, IN const ib_smp_t * p_src_smp,
			       IN ib_net16_t status,
			       IN const uint8_t * p_payload,
			       OUT ib_smp_t * p_dest_smp)
{
	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_dest_smp);
	CL_ASSERT(p_src_smp);
	CL_ASSERT(!ib_smp_is_response(p_src_smp));

	*p_dest_smp = *p_src_smp;
	if (p_src_smp->method == IB_MAD_METHOD_GET ||
	    p_src_smp->method == IB_MAD_METHOD_SET) {
		p_dest_smp->method = IB_MAD_METHOD_GET_RESP;
		p_dest_smp->status = status;
	} else if (p_src_smp->method == IB_MAD_METHOD_TRAP) {
		p_dest_smp->method = IB_MAD_METHOD_TRAP_REPRESS;
		p_dest_smp->status = 0;
	} else {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 1302: "
			"src smp method unsupported 0x%X\n", p_src_smp->method);
		goto Exit;
	}

	if (p_src_smp->mgmt_class == IB_MCLASS_SUBN_DIR)
		p_dest_smp->status |= IB_SMP_DIRECTION;

	p_dest_smp->dr_dlid = p_dest_smp->dr_slid;
	p_dest_smp->dr_slid = p_dest_smp->dr_dlid;
	memcpy(&p_dest_smp->data, p_payload, IB_SMP_DATA_SIZE);

Exit:
	OSM_LOG_EXIT(sm->p_log);
}