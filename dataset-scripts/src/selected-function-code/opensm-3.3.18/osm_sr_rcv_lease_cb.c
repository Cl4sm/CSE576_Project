void osm_sr_rcv_lease_cb(IN void *context)
{
	osm_sa_t *sa = context;
	cl_list_item_t *p_list_item;
	cl_list_item_t *p_next_list_item;
	osm_svcr_t *p_svcr;
	uint32_t curr_time;
	uint32_t elapsed_time;
	uint32_t trim_time = 20;	/*  maxiaml timer refresh is 20 seconds */

	OSM_LOG_ENTER(sa->p_log);

	cl_plock_excl_acquire(sa->p_lock);

	p_list_item = cl_qlist_head(&sa->p_subn->sa_sr_list);

	while (p_list_item != cl_qlist_end(&sa->p_subn->sa_sr_list)) {
		p_svcr = (osm_svcr_t *) p_list_item;

		if (p_svcr->service_record.service_lease == 0xFFFFFFFF) {
			p_list_item = cl_qlist_next(p_list_item);
			continue;
		}

		/* current time in seconds */
		curr_time = cl_get_time_stamp_sec();
		/* elapsed time from last modify */
		elapsed_time = curr_time - p_svcr->modified_time;
		/* but it can not be less then 1 */
		if (elapsed_time < 1)
			elapsed_time = 1;

		if (elapsed_time < p_svcr->lease_period) {
			/*
			   Just update the service lease period
			   note: for simplicity we work with a uint32_t field
			   external to the network order lease_period of the MAD
			 */
			p_svcr->lease_period -= elapsed_time;

			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Remaining time for Service Name:%s is:0x%X\n",
				p_svcr->service_record.service_name,
				p_svcr->lease_period);

			p_svcr->modified_time = curr_time;

			/* Update the trim timer */
			if (trim_time > p_svcr->lease_period) {
				trim_time = p_svcr->lease_period;
				if (trim_time < 1)
					trim_time = 1;
			}

			p_list_item = cl_qlist_next(p_list_item);
			continue;

		} else {
			p_next_list_item = cl_qlist_next(p_list_item);

			/* Remove the service Record */
			osm_svcr_remove_from_db(sa->p_subn, sa->p_log, p_svcr);

			osm_svcr_delete(p_svcr);

			p_list_item = p_next_list_item;
			continue;
		}
	}

	/* Release the Lock */
	cl_plock_release(sa->p_lock);

	if (trim_time != 0xFFFFFFFF) {
		cl_timer_trim(&sa->sr_timer, trim_time * 1000);	/* Convert to milli seconds */
	}

	OSM_LOG_EXIT(sa->p_log);
}