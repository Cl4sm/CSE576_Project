cl_status_t cl_event_wheel_reg(IN cl_event_wheel_t * const p_event_wheel,
			       IN const uint64_t key,
			       IN const uint64_t aging_time_usec,
			       IN cl_pfn_event_aged_cb_t pfn_callback,
			       IN void *const context)
{
	cl_event_wheel_reg_info_t *p_event;
	uint64_t timeout;
	uint32_t to;
	cl_status_t cl_status = CL_SUCCESS;
	cl_list_item_t *prev_event_list_item;
	cl_map_item_t *p_map_item;

	/* Get the lock on the manager */
	cl_spinlock_acquire(&(p_event_wheel->lock));

	cl_event_wheel_dump(p_event_wheel);

	/* Make sure such a key does not exists */
	p_map_item = cl_qmap_get(&p_event_wheel->events_map, key);
	if (p_map_item != cl_qmap_end(&p_event_wheel->events_map)) {
		CL_DBG("cl_event_wheel_reg: Already exists key:0x%"
		       PRIx64 "\n", key);

		/* already there - remove it from the list as it is getting a new time */
		p_event =
		    PARENT_STRUCT(p_map_item, cl_event_wheel_reg_info_t,
				  map_item);

		/* remove the item from the qlist */
		cl_qlist_remove_item(&p_event_wheel->events_wheel,
				     &p_event->list_item);
		/* and the qmap */
		cl_qmap_remove_item(&p_event_wheel->events_map,
				    &p_event->map_item);
	} else {
		/* make a new one */
		p_event = (cl_event_wheel_reg_info_t *)
		    malloc(sizeof(cl_event_wheel_reg_info_t));
		p_event->num_regs = 0;
	}

	p_event->key = key;
	p_event->aging_time = aging_time_usec;
	p_event->pfn_aged_callback = pfn_callback;
	p_event->context = context;
	p_event->num_regs++;

	CL_DBG("cl_event_wheel_reg: Registering event key:0x%" PRIx64
	       " aging in %u [msec]\n", p_event->key,
	       (uint32_t) ((p_event->aging_time - cl_get_time_stamp()) / 1000));

	/* If the list is empty - need to start the timer */
	if (cl_is_qlist_empty(&p_event_wheel->events_wheel)) {
		/* Edward Bortnikov 03/29/2003
		 * ++TBD Consider moving the timer manipulation behind the list manipulation.
		 */

		/* calculate the new timeout */
		timeout =
		    (p_event->aging_time - cl_get_time_stamp() + 500) / 1000;

		/* stop the timer if it is running */

		/* Edward Bortnikov 03/29/2003
		 * Don't call cl_timer_stop() because it spins forever.
		 * cl_timer_start() will invoke cl_timer_stop() by itself.
		 *
		 * The problematic scenario is when __cl_event_wheel_callback()
		 * is in race condition with this code. It sets timer.in_timer_cb
		 * to TRUE and then blocks on p_event_wheel->lock. Following this,
		 * the call to cl_timer_stop() hangs. Following this, the whole system
		 * enters into a deadlock.
		 *
		 * cl_timer_stop(&p_event_wheel->timer);
		 */

		/* The timeout for the cl_timer_start should be given as uint32_t.
		   if there is an overflow - warn about it. */
		to = (uint32_t) timeout;
		if (timeout > (uint32_t) timeout) {
			to = 0xffffffff;	/* max 32 bit timer */
			CL_DBG("cl_event_wheel_reg: timeout requested is "
			       "too large. Using timeout: %u\n", to);
		}

		/* start the timer to the timeout [msec] */
		cl_status = cl_timer_start(&p_event_wheel->timer, to);
		if (cl_status != CL_SUCCESS) {
			CL_DBG("cl_event_wheel_reg : ERR 6203: "
			       "Failed to start timer\n");
			goto Exit;
		}
	}

	/* insert the object to the qlist and the qmap */

	/* BUT WE MUST INSERT IT IN A SORTED MANNER */
	prev_event_list_item =
	    cl_qlist_find_from_tail(&p_event_wheel->events_wheel,
				    __event_will_age_before,
				    &p_event->aging_time);

	cl_qlist_insert_next(&p_event_wheel->events_wheel,
			     prev_event_list_item, &p_event->list_item);

	cl_qmap_insert(&p_event_wheel->events_map, key, &(p_event->map_item));

Exit:
	cl_spinlock_release(&p_event_wheel->lock);

	return cl_status;
}