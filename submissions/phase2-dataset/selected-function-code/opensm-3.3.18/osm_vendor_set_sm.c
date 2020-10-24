void osm_vendor_set_sm(IN osm_bind_handle_t h_bind, IN boolean_t is_sm_val)
{
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osm_vendor_t const *p_vend = p_bo->p_vendor;
	VAPI_ret_t status;
	VAPI_hca_attr_t attr_mod;
	VAPI_hca_attr_mask_t attr_mask;

	OSM_LOG_ENTER(p_vend->p_log);

	memset(&attr_mod, 0, sizeof(attr_mod));
	memset(&attr_mask, 0, sizeof(attr_mask));

	attr_mod.is_sm = is_sm_val;
	attr_mask = HCA_ATTR_IS_SM;

	status =
	    VAPI_modify_hca_attr(p_bo->hca_hndl, p_bo->port_num, &attr_mod,
				 &attr_mask);
	if (status != VAPI_OK) {
		osm_log(p_vend->p_log, OSM_LOG_ERROR,
			"osm_vendor_set_sm: ERR 7312: "
			"Unable set 'IS_SM' bit to:%u in port attributes (%d).\n",
			is_sm_val, status);
	}

	OSM_LOG_EXIT(p_vend->p_log);
}