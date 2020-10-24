uint32_t cl_event_wheel_num_regs(IN cl_event_wheel_t * const p_event_wheel,
				 IN uint64_t key)
{

	cl_event_wheel_reg_info_t *p_event;
	cl_map_item_t *p_map_item;
	uint32_t num_regs = 0;

	/* try to find the key in the map */
	CL_DBG("cl_event_wheel_num_regs: Looking for key:0x%" PRIx64 "\n", key);

	cl_spinlock_acquire(&p_event_wheel->lock);
	p_map_item = cl_qmap_get(&p_event_wheel->events_map, key);
	if (p_map_item != cl_qmap_end(&p_event_wheel->events_map)) {
		/* ok so we can simply return it's num_regs */
		p_event =
		    PARENT_STRUCT(p_map_item, cl_event_wheel_reg_info_t,
				  map_item);
		num_regs = p_event->num_regs;
	}

	cl_spinlock_release(&p_event_wheel->lock);
	return (num_regs);
}