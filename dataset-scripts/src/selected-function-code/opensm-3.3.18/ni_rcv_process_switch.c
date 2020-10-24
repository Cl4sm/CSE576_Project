static void ni_rcv_process_switch(IN osm_sm_t * sm, IN osm_node_t * p_node,
				  IN const osm_madw_t * p_madw)
{
	ib_api_status_t status = IB_SUCCESS;
	osm_physp_t *p_physp;
	osm_madw_context_t context;
	osm_dr_path_t *path;
	ib_smp_t *p_smp;

	OSM_LOG_ENTER(sm->p_log);

	p_smp = osm_madw_get_smp_ptr(p_madw);

	p_physp = osm_node_get_physp_ptr(p_node, 0);
	/* update DR path of already initialized switch port 0 */
	path = osm_physp_get_dr_path_ptr(p_physp);
	osm_dr_path_init(path, p_smp->hop_count, p_smp->initial_path);

	context.si_context.node_guid = osm_node_get_node_guid(p_node);
	context.si_context.set_method = FALSE;
	context.si_context.light_sweep = FALSE;
	context.si_context.lft_top_change = FALSE;

	/* Request a SwitchInfo attribute */
	status = osm_req_get(sm, path, IB_MAD_ATTR_SWITCH_INFO, 0, TRUE, 0,
			     CL_DISP_MSGID_NONE, &context);
	if (status != IB_SUCCESS)
		/* continue despite error */
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D06: "
			"Failure initiating SwitchInfo request (%s)\n",
			ib_get_err_str(status));

	OSM_LOG_EXIT(sm->p_log);
}