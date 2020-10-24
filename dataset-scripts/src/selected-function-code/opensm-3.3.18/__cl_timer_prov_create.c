cl_status_t __cl_timer_prov_create(void)
{
	CL_ASSERT(gp_timer_prov == NULL);

	gp_timer_prov = malloc(sizeof(cl_timer_prov_t));
	if (!gp_timer_prov)
		return (CL_INSUFFICIENT_MEMORY);
	else
		memset(gp_timer_prov, 0, sizeof(cl_timer_prov_t));

	cl_qlist_init(&gp_timer_prov->queue);

	pthread_mutex_init(&gp_timer_prov->mutex, NULL);
	pthread_cond_init(&gp_timer_prov->cond, NULL);

	if (pthread_create(&gp_timer_prov->thread, NULL,
			   __cl_timer_prov_cb, NULL)) {
		__cl_timer_prov_destroy();
		return (CL_ERROR);
	}

	return (CL_SUCCESS);
}