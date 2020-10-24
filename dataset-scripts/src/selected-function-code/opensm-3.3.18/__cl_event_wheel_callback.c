static void __cl_event_wheel_callback(IN void *context)
{
	cl_event_wheel_t *p_event_wheel = (cl_event_wheel_t *) context;
	cl_list_item_t *p_list_item, *p_prev_event_list_item;
	cl_list_item_t *p_list_next_item;
	cl_event_wheel_reg_info_t *p_event;
	uint64_t current_time;
	uint64_t next_aging_time;
	uint32_t new_timeout;
	cl_status_t cl_status;

	/* might be during closing ...  */
	if (p_event_wheel->closing)
		return;

	current_time = cl_get_time_stamp();

	if (NULL != p_event_wheel->p_external_lock)

		/* Take care of the order of acquiring locks to avoid the deadlock!
		 * The external lock goes first.
		 */
		cl_spinlock_acquire(p_event_wheel->p_external_lock);

	cl_spinlock_acquire(&p_event_wheel->lock);

	p_list_item = cl_qlist_head(&p_event_wheel->events_wheel);
	if (p_list_item == cl_qlist_end(&p_event_wheel->events_wheel))
		/* the list is empty - nothing to do */
		goto Exit;

	/* we found such an item.  get the p_event */
	p_event =
	    PARENT_STRUCT(p_list_item, cl_event_wheel_reg_info_t, list_item);

	while (p_event->aging_time <= current_time) {
		/* this object has aged - invoke it's callback */
		if (p_event->pfn_aged_callback)
			next_aging_time =
			    p_event->pfn_aged_callback(p_event->key,
						       p_event->num_regs,
						       p_event->context);
		else
			next_aging_time = 0;

		/* point to the next object in the wheel */
		p_list_next_item = cl_qlist_next(p_list_item);

		/* We need to retire the event if the next aging time passed */
		if (next_aging_time < current_time) {
			/* remove it from the map */
			cl_qmap_remove_item(&p_event_wheel->events_map,
					    &(p_event->map_item));

			/* pop p_event from the wheel */
			cl_qlist_remove_head(&p_event_wheel->events_wheel);

			/* delete the event info object - allocated by cl_event_wheel_reg */
			free(p_event);
		} else {
			/* update the required aging time */
			p_event->aging_time = next_aging_time;
			p_event->num_regs++;

			/* do not remove from the map  - but remove from the list head and
			   place in the correct position */

			/* pop p_event from the wheel */
			cl_qlist_remove_head(&p_event_wheel->events_wheel);

			/* find the event that ages just before */
			p_prev_event_list_item =
			    cl_qlist_find_from_tail(&p_event_wheel->
						    events_wheel,
						    __event_will_age_before,
						    &p_event->aging_time);

			/* insert just after */
			cl_qlist_insert_next(&p_event_wheel->events_wheel,
					     p_prev_event_list_item,
					     &p_event->list_item);

			/* as we have modified the list - restart from first item: */
			p_list_next_item =
			    cl_qlist_head(&p_event_wheel->events_wheel);
		}

		/* advance to next event */
		p_list_item = p_list_next_item;
		if (p_list_item == cl_qlist_end(&p_event_wheel->events_wheel))
			/* the list is empty - nothing to do */
			break;

		/* get the p_event */
		p_event =
		    PARENT_STRUCT(p_list_item, cl_event_wheel_reg_info_t,
				  list_item);
	}

	/* We need to restart the timer only if the list is not empty now */
	if (p_list_item != cl_qlist_end(&p_event_wheel->events_wheel)) {
		/* get the p_event */
		p_event =
		    PARENT_STRUCT(p_list_item, cl_event_wheel_reg_info_t,
				  list_item);

		/* start the timer to the timeout [msec] */
		new_timeout =
		    (uint32_t) ((p_event->aging_time - current_time + 500) / 1000);
		CL_DBG("__cl_event_wheel_callback: Restart timer in: "
		       "%u [msec]\n", new_timeout);
		cl_status = cl_timer_start(&p_event_wheel->timer, new_timeout);
		if (cl_status != CL_SUCCESS) {
			CL_DBG("__cl_event_wheel_callback : ERR 6200: "
			       "Failed to start timer\n");
		}
	}

	/* release the lock */
Exit:
	cl_spinlock_release(&p_event_wheel->lock);
	if (NULL != p_event_wheel->p_external_lock)
		cl_spinlock_release(p_event_wheel->p_external_lock);
}