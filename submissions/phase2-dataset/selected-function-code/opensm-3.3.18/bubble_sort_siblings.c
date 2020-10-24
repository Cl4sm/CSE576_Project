static inline void
bubble_sort_siblings(ftree_port_group_t ** p_group_array, uint32_t nmemb)
{
	uint32_t i = 0;
	uint32_t j = 0;
	ftree_port_group_t *tmp = p_group_array[0];

	/* While we did modifications on the array order */
	/* i may grew above array length but next loop will fail and tmp will be null for the next time
	 * this way we save a test i < nmemb for each pass through the loop */
	for (i = 0; tmp != NULL; i++) {
		/* Assume the array is orderd */
		tmp = NULL;
		/* Comparing elements j and j-1 */
		for (j = 1; j < (nmemb - i); j++) {
			/* If they are the wrong way around */
			if (port_group_compare_load_up(p_group_array[j],
						       p_group_array[j - 1]) < 0) {
				/* We invert them */
				tmp = p_group_array[j - 1];
				p_group_array[j - 1] = p_group_array[j];
				p_group_array[j] = tmp;
			}
		}
	}
}