void osmv_transport_done(IN const osm_bind_handle_t h_bind)
{
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osm_log_t *p_log = p_bo->p_vendor->p_log;
	osmv_IBMGT_transport_mgr_t *p_mgr;
	osmv_IBMGT_transport_info_t *p_tpot_info;
	IB_MGT_ret_t ret;
	cl_list_obj_t *p_obj = NULL;
	cl_list_item_t *p_item, *p_item_tmp;
	int i;
	cl_qlist_t *p_list = NULL;

	OSM_LOG_ENTER(p_log);

	CL_ASSERT(p_bo);

	/* First of all - zero out the magic_ptr, so if a callback is called -
	   it'll know that we are currently closing down, and will not handle the
	   mad. */
	p_bo->magic_ptr = 0;

	p_mgr = (osmv_IBMGT_transport_mgr_t *) (p_bo->p_transp_mgr);
	p_tpot_info =
	    (osmv_IBMGT_transport_info_t *) (p_bo->p_vendor->p_transport_info);

	switch (p_mgr->mad_type) {
	case IB_MGT_SMI:
		p_list = p_tpot_info->p_smi_list;

		/* remove from the bindings list */
		p_item = cl_qlist_head(p_list);
		while (p_item != cl_qlist_end(p_list)) {
			p_obj = PARENT_STRUCT(p_item, cl_list_obj_t, list_item);
			if (cl_qlist_obj(p_obj) == h_bind) {
				break;
			}
			p_item_tmp = cl_qlist_next(p_item);
			p_item = p_item_tmp;
		}

		CL_ASSERT(p_item != cl_qlist_end(p_list));
		cl_qlist_remove_item(p_list, p_item);
		if (p_obj)
			free(p_obj);

		/* no one is binded to smi anymore - we can free the list, unbind & realease the hndl */
		if (cl_is_qlist_empty(p_list) == TRUE) {
			free(p_list);
			p_list = NULL;

			ret = IB_MGT_unbind_sm(p_tpot_info->smi_h);
			if (ret != IB_MGT_OK) {
				osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
					"osmv_transport_done: ERR 7212: "
					"Failed to unbind sm\n");
			}

			ret = IB_MGT_release_handle(p_tpot_info->smi_h);
			if (ret != IB_MGT_OK) {
				osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
					"osmv_transport_done: ERR 7213: "
					"Failed to release smi handle\n");
			}
			p_tpot_info->smi_h = 0xffffffff;
		}
		break;

	case IB_MGT_GSI:
		p_list = p_tpot_info->gsi_mgmt_lists[p_mgr->mgmt_class];
		/* remove from the bindings list */
		p_item = cl_qlist_head(p_list);
		while (p_item != cl_qlist_end(p_list)) {
			p_obj = PARENT_STRUCT(p_item, cl_list_obj_t, list_item);
			if (cl_qlist_obj(p_obj) == h_bind) {
				break;
			}
			p_item_tmp = cl_qlist_next(p_item);
			p_item = p_item_tmp;
		}

		CL_ASSERT(p_item != cl_qlist_end(p_list));
		cl_qlist_remove_item(p_list, p_item);
		if (p_obj)
			free(p_obj);

		/* no one is binded to this class anymore - we can free the list and unbind this class */
		if (cl_is_qlist_empty(p_list) == TRUE) {
			free(p_list);
			p_list = NULL;

			ret =
			    IB_MGT_unbind_gsi_class(p_tpot_info->gsi_h,
						    p_mgr->mgmt_class);
			if (ret != IB_MGT_OK) {
				osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
					"osmv_transport_done: ERR 7214: "
					"Failed to unbind gsi class\n");
			}
		}

		/* all the mgmt classes are unbinded - release gsi handle */
		for (i = 0; i < 15; i++) {
			if (p_tpot_info->gsi_mgmt_lists[i] != NULL) {
				break;
			}
		}

		if (i == 15) {
			ret = IB_MGT_release_handle(p_tpot_info->gsi_h);
			if (ret != IB_MGT_OK) {
				osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
					"osmv_transport_done: ERR 7215: "
					"Failed to release gsi handle\n");
			}
			p_tpot_info->gsi_h = 0xffffffff;
		}
	}			/* end switch */

	free(p_mgr);
}