void cl_event_wheel_unreg(IN cl_event_wheel_t * const p_event_wheel,
			  IN uint64_t key)
{
	cl_event_wheel_reg_info_t *p_event;
	cl_map_item_t *p_map_item;

	CL_DBG("cl_event_wheel_unreg: " "Removing key:0x%" PRIx64 "\n", key);

	cl_spinlock_acquire(&p_event_wheel->lock);
	p_map_item = cl_qmap_get(&p_event_wheel->events_map, key);
	if (p_map_item != cl_qmap_end(&p_event_wheel->events_map)) {
		/* we found such an item. */
		p_event =
		    PARENT_STRUCT(p_map_item, cl_event_wheel_reg_info_t,
				  map_item);

		/* remove the item from the qlist */
		cl_qlist_remove_item(&p_event_wheel->events_wheel,
				     &(p_event->list_item));
		/* remove the item from the qmap */
		cl_qmap_remove_item(&p_event_wheel->events_map,
				    &(p_event->map_item));

		CL_DBG("cl_event_wheel_unreg: Removed key:0x%" PRIx64 "\n",
		       key);

		/* free the item */
		free(p_event);
	} else {
		CL_DBG("cl_event_wheel_unreg: did not find key:0x%" PRIx64
		       "\n", key);
	}

	cl_spinlock_release(&p_event_wheel->lock);
}