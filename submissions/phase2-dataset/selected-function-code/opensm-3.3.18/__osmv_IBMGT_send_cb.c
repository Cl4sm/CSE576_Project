void
__osmv_IBMGT_send_cb(IN IB_MGT_mad_hndl_t mad_hndl,
		     IN u_int64_t wrid,
		     IN IB_comp_status_t status, IN void *private_ctx_p)
{
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) CAST_P2LONG(wrid);

	osmv_IBMGT_transport_mgr_t *p_mgr =
	    (osmv_IBMGT_transport_mgr_t *) p_bo->p_transp_mgr;

	/* Make sure the p_bo object is still relevant */
	if (p_bo->magic_ptr != p_bo)
		return;

	/* we assume that each send on a bind object is synchronized, and no paralel sends
	   from diffrent threads with same object can be made */
	if (status == IB_COMP_SUCCESS) {
		p_mgr->is_send_ok = TRUE;
	} else
		p_mgr->is_send_ok = FALSE;
	cl_event_signal(&p_mgr->send_done);

}