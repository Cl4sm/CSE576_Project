static cl_status_t __cl_timer_find(IN const cl_list_item_t * const p_list_item,
				   IN void *const context)
{
	cl_timer_t *p_in_list;
	cl_timer_t *p_new;

	CL_ASSERT(p_list_item);
	CL_ASSERT(context);

	p_in_list = (cl_timer_t *) p_list_item;
	p_new = (cl_timer_t *) context;

	CL_ASSERT(p_in_list->state == CL_INITIALIZED);
	CL_ASSERT(p_new->state == CL_INITIALIZED);

	CL_ASSERT(p_in_list->timer_state == CL_TIMER_QUEUED);

	if (__cl_timer_is_earlier(&p_in_list->timeout, &p_new->timeout))
		return (CL_SUCCESS);

	return (CL_NOT_FOUND);
}