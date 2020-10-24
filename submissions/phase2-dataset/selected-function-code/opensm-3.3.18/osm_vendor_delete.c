void osm_vendor_delete(IN osm_vendor_t ** const pp_vend)
{
	cl_list_item_t *p_item;
	cl_list_obj_t *p_obj;
	osm_bind_handle_t bind_h;
	osm_log_t *p_log;

	OSM_LOG_ENTER((*pp_vend)->p_log);
	p_log = (*pp_vend)->p_log;

	/* go over the bind handles , unbind them and remove from list */
	p_item = cl_qlist_remove_head(&((*pp_vend)->bind_handles));
	while (p_item != cl_qlist_end(&((*pp_vend)->bind_handles))) {

		p_obj = PARENT_STRUCT(p_item, cl_list_obj_t, list_item);
		bind_h = (osm_bind_handle_t *) cl_qlist_obj(p_obj);
		osm_log(p_log, OSM_LOG_DEBUG,
			"osm_vendor_delete: unbinding bind_h:%p \n", bind_h);

		__osm_vendor_internal_unbind(bind_h);

		free(p_obj);
		/*removing from list */
		p_item = cl_qlist_remove_head(&((*pp_vend)->bind_handles));
	}

	if (NULL != ((*pp_vend)->p_transport_info)) {
		free((*pp_vend)->p_transport_info);
		(*pp_vend)->p_transport_info = NULL;
	}

	/* remove the packet randomizer object */
	if ((*pp_vend)->run_randomizer == TRUE)
		osm_pkt_randomizer_destroy(&((*pp_vend)->p_pkt_randomizer),
					   p_log);

	free(*pp_vend);
	*pp_vend = NULL;

	OSM_LOG_EXIT(p_log);
}