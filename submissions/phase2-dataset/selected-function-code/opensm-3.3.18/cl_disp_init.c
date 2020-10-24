cl_status_t cl_disp_init(IN cl_dispatcher_t * const p_disp,
			 IN const uint32_t thread_count,
			 IN const char *const name)
{
	cl_status_t status;

	CL_ASSERT(p_disp);

	cl_disp_construct(p_disp);

	status = cl_spinlock_init(&p_disp->lock);
	if (status != CL_SUCCESS) {
		cl_disp_destroy(p_disp);
		return (status);
	}

	/* Specify no upper limit to the number of messages in the pool */
	status = cl_qpool_init(&p_disp->msg_pool, CL_DISP_INITIAL_MSG_COUNT,
			       0, CL_DISP_MSG_GROW_SIZE, sizeof(cl_disp_msg_t),
			       NULL, NULL, NULL);
	if (status != CL_SUCCESS) {
		cl_disp_destroy(p_disp);
		return (status);
	}

	status = cl_ptr_vector_init(&p_disp->reg_vec, CL_DISP_INITIAL_REG_COUNT,
				    CL_DISP_REG_GROW_SIZE);
	if (status != CL_SUCCESS) {
		cl_disp_destroy(p_disp);
		return (status);
	}

	status = cl_thread_pool_init(&p_disp->worker_threads, thread_count,
				     __cl_disp_worker, p_disp, name);
	if (status != CL_SUCCESS)
		cl_disp_destroy(p_disp);

	return (status);
}