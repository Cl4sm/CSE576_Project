static void query_sm_info(cl_map_item_t * item, void *cxt)
{
	osm_madw_context_t context;
	osm_remote_sm_t *r_sm = cl_item_obj(item, r_sm, map_item);
	osm_sm_t *sm = cxt;
	ib_api_status_t ret;
	osm_port_t *p_port;

	p_port= osm_get_port_by_guid(sm->p_subn, r_sm->smi.guid);
	if (p_port == NULL) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3340: "
			"No port object on given sm object\n");
		return;
        }

	context.smi_context.port_guid = r_sm->smi.guid;
	context.smi_context.set_method = FALSE;
	context.smi_context.light_sweep = TRUE;

	ret = osm_req_get(sm, osm_physp_get_dr_path_ptr(p_port->p_physp),
			  IB_MAD_ATTR_SM_INFO, 0, FALSE,
			  ib_port_info_get_m_key(&p_port->p_physp->port_info),
			  CL_DISP_MSGID_NONE, &context);
	if (ret != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3314: "
			"Failure requesting SMInfo (%s)\n",
			ib_get_err_str(ret));
}