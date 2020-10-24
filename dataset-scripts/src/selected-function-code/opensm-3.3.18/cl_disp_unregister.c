void cl_disp_unregister(IN const cl_disp_reg_handle_t handle)
{
	cl_disp_reg_info_t *p_reg;
	cl_dispatcher_t *p_disp;

	if (handle == CL_DISP_INVALID_HANDLE)
		return;

	p_reg = (cl_disp_reg_info_t *) handle;
	p_disp = p_reg->p_disp;
	CL_ASSERT(p_disp);

	cl_spinlock_acquire(&p_disp->lock);
	/*
	 * Clear the registrant vector entry.  This will cause any further
	 * post calls to fail.
	 */
	if (p_reg->msg_id != CL_DISP_MSGID_NONE) {
		CL_ASSERT(p_reg->msg_id <
			  cl_ptr_vector_get_size(&p_disp->reg_vec));
		cl_ptr_vector_set(&p_disp->reg_vec, p_reg->msg_id, NULL);
	}
	cl_spinlock_release(&p_disp->lock);

	while (p_reg->ref_cnt > 0)
		cl_thread_suspend(1);

	cl_spinlock_acquire(&p_disp->lock);
	/* Remove the registrant from the list. */
	cl_qlist_remove_item(&p_disp->reg_list, (cl_list_item_t *) p_reg);
	/* Return the registration info to the pool */
	free(p_reg);

	cl_spinlock_release(&p_disp->lock);
}