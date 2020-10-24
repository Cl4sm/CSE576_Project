static inline int port_group_compare_load_down(const ftree_port_group_t * p1,
					       const ftree_port_group_t * p2)
{
	int temp = p1->counter_down - p2->counter_down;
	if (temp > 0)
		return 1;
	if (temp < 0)
		return -1;

	/* Find the less loaded remote sw and choose this one */
	do {
		uint32_t load1 =
		    find_lowest_loaded_group_on_sw(p1->remote_hca_or_sw.p_sw);
		uint32_t load2 =
		    find_lowest_loaded_group_on_sw(p2->remote_hca_or_sw.p_sw);
		temp = load1 - load2;
		if (temp > 0)
			return 1;
	} while (0);
	/* If they are both equal, choose the lowest index */
	return compare_port_groups_by_remote_switch_index(&p1, &p2);
}