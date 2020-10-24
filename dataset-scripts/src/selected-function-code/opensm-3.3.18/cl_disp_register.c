cl_disp_reg_handle_t cl_disp_register(IN cl_dispatcher_t * const p_disp,
				      IN const cl_disp_msgid_t msg_id,
				      IN cl_pfn_msgrcv_cb_t pfn_callback
				      OPTIONAL,
				      IN const void *const context OPTIONAL)
{
	cl_disp_reg_info_t *p_reg;
	cl_status_t status;

	CL_ASSERT(p_disp);

	/* Check that the requested registrant ID is available. */
	cl_spinlock_acquire(&p_disp->lock);
	if ((msg_id != CL_DISP_MSGID_NONE) &&
	    (msg_id < cl_ptr_vector_get_size(&p_disp->reg_vec)) &&
	    (cl_ptr_vector_get(&p_disp->reg_vec, msg_id))) {
		cl_spinlock_release(&p_disp->lock);
		return (NULL);
	}

	/* Get a registration info from the pool. */
	p_reg = (cl_disp_reg_info_t *) malloc(sizeof(cl_disp_reg_info_t));
	if (!p_reg) {
		cl_spinlock_release(&p_disp->lock);
		return (NULL);
	} else {
		memset(p_reg, 0, sizeof(cl_disp_reg_info_t));
	}

	p_reg->p_disp = p_disp;
	p_reg->ref_cnt = 0;
	p_reg->pfn_rcv_callback = pfn_callback;
	p_reg->context = context;
	p_reg->msg_id = msg_id;

	/* Insert the registration in the list. */
	cl_qlist_insert_tail(&p_disp->reg_list, (cl_list_item_t *) p_reg);

	/* Set the array entry to the registrant. */
	/* The ptr_vector grow automatically as necessary. */
	if (msg_id != CL_DISP_MSGID_NONE) {
		status = cl_ptr_vector_set(&p_disp->reg_vec, msg_id, p_reg);
		if (status != CL_SUCCESS) {
			free(p_reg);
			cl_spinlock_release(&p_disp->lock);
			return (NULL);
		}
	}

	cl_spinlock_release(&p_disp->lock);

	return (p_reg);
}