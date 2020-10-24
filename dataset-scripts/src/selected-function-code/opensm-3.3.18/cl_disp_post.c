cl_status_t cl_disp_post(IN const cl_disp_reg_handle_t handle,
			 IN const cl_disp_msgid_t msg_id,
			 IN const void *const p_data,
			 IN cl_pfn_msgdone_cb_t pfn_callback OPTIONAL,
			 IN const void *const context OPTIONAL)
{
	cl_disp_reg_info_t *p_src_reg = (cl_disp_reg_info_t *) handle;
	cl_disp_reg_info_t *p_dest_reg;
	cl_dispatcher_t *p_disp;
	cl_disp_msg_t *p_msg;

	p_disp = handle->p_disp;
	CL_ASSERT(p_disp);
	CL_ASSERT(msg_id != CL_DISP_MSGID_NONE);

	cl_spinlock_acquire(&p_disp->lock);
	/* Check that the recipient exists. */
	p_dest_reg = cl_ptr_vector_get(&p_disp->reg_vec, msg_id);
	if (!p_dest_reg) {
		cl_spinlock_release(&p_disp->lock);
		return (CL_NOT_FOUND);
	}

	/* Get a free message from the pool. */
	p_msg = (cl_disp_msg_t *) cl_qpool_get(&p_disp->msg_pool);
	if (!p_msg) {
		cl_spinlock_release(&p_disp->lock);
		return (CL_INSUFFICIENT_MEMORY);
	}

	/* Initialize the message */
	p_msg->p_src_reg = p_src_reg;
	p_msg->p_dest_reg = p_dest_reg;
	p_msg->p_data = p_data;
	p_msg->pfn_xmt_callback = pfn_callback;
	p_msg->context = context;
	p_msg->in_time = cl_get_time_stamp();

	/*
	 * Increment the sender's reference count if they request a completion
	 * notification.
	 */
	if (pfn_callback)
		cl_atomic_inc(&p_src_reg->ref_cnt);

	/* Increment the recipient's reference count. */
	cl_atomic_inc(&p_dest_reg->ref_cnt);

	/* Queue the message in the FIFO. */
	cl_qlist_insert_tail(&p_disp->msg_fifo, (cl_list_item_t *) p_msg);
	cl_spinlock_release(&p_disp->lock);

	/* Signal the thread pool that there is work to be done. */
	cl_thread_pool_signal(&p_disp->worker_threads);
	return (CL_SUCCESS);
}