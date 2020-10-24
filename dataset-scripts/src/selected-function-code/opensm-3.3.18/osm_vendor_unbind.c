void osm_vendor_unbind(IN osm_bind_handle_t h_bind)
{
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osm_log_t *p_log = p_bo->p_vendor->p_log;
	cl_list_obj_t *p_obj = NULL;
	cl_list_item_t *p_item, *p_item_tmp;
	cl_qlist_t *const p_bh_list =
	    (cl_qlist_t * const)&p_bo->p_vendor->bind_handles;

	OSM_LOG_ENTER(p_log);

	/* go over all the items in the list and remove the specific item */
	p_item = cl_qlist_head(p_bh_list);
	while (p_item != cl_qlist_end(p_bh_list)) {
		p_obj = PARENT_STRUCT(p_item, cl_list_obj_t, list_item);
		if (cl_qlist_obj(p_obj) == h_bind) {
			break;
		}
		p_item_tmp = cl_qlist_next(p_item);
		p_item = p_item_tmp;
	}

	CL_ASSERT(p_item != cl_qlist_end(p_bh_list));

	cl_qlist_remove_item(p_bh_list, p_item);
	if (p_obj)
		free(p_obj);

	if (h_bind != 0) {
		__osm_vendor_internal_unbind(h_bind);
	}

	OSM_LOG_EXIT(p_log);
}