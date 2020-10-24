static void smi_rcv_process_set_response(IN osm_sm_t * sm,
					 IN const osm_madw_t * p_madw)
{
	const ib_smp_t *p_smp;
	const ib_sm_info_t *p_smi;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	if (ib_smp_get_status(p_smp)) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"MAD status 0x%x received\n",
			cl_ntoh16(ib_smp_get_status(p_smp)));
		goto Exit;
	}

	if (p_smp->method != IB_MAD_METHOD_GET_RESP) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F16: "
			"Unsupported response method 0x%X\n", p_smp->method);
		goto Exit;
	}

	p_smi = ib_smp_get_payload_ptr(p_smp);
	osm_dump_sm_info_v2(sm->p_log, p_smi, FILE_ID, OSM_LOG_DEBUG);

	/* Check the AttributeModifier */
	if (p_smp->attr_mod != IB_SMINFO_ATTR_MOD_HANDOVER) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F17: "
			"Unsupported attribute modifier 0x%X, "
			"expected ATTR_MOD_HANDOVER\n",
			p_smp->attr_mod);
		goto Exit;
	}

	/* This is a response on a HANDOVER request - Nothing to do. */

Exit:
	OSM_LOG_EXIT(sm->p_log);
}