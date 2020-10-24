void cl_event_wheel_destroy(IN cl_event_wheel_t * const p_event_wheel)
{
	cl_list_item_t *p_list_item;
	cl_map_item_t *p_map_item;
	cl_event_wheel_reg_info_t *p_event;

	/* we need to get a lock */
	cl_spinlock_acquire(&p_event_wheel->lock);

	cl_event_wheel_dump(p_event_wheel);

	/* go over all the items in the list and remove them */
	p_list_item = cl_qlist_remove_head(&p_event_wheel->events_wheel);
	while (p_list_item != cl_qlist_end(&p_event_wheel->events_wheel)) {
		p_event =
		    PARENT_STRUCT(p_list_item, cl_event_wheel_reg_info_t,
				  list_item);

		CL_DBG("cl_event_wheel_destroy: Found outstanding event"
		       " key:<0x%" PRIx64 ">\n", p_event->key);

		/* remove it from the map */
		p_map_item = &(p_event->map_item);
		cl_qmap_remove_item(&p_event_wheel->events_map, p_map_item);
		free(p_event);	/* allocated by cl_event_wheel_reg */
		p_list_item =
		    cl_qlist_remove_head(&p_event_wheel->events_wheel);
	}

	/* destroy the timer */
	cl_timer_destroy(&p_event_wheel->timer);

	/* destroy the lock (this should be done without releasing - we don't want
	   any other run to grab the lock at this point. */
	cl_spinlock_release(&p_event_wheel->lock);
	cl_spinlock_destroy(&(p_event_wheel->lock));
}