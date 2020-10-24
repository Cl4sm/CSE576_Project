static int generate_routing_func_for_mst(lash_t * p_lash, int sw_id,
					 reachable_dest_t ** destinations)
{
	int i, next_switch;
	switch_t *sw = p_lash->switches[sw_id];
	int num_channels = sw->used_channels;
	reachable_dest_t *dest, *i_dest, *concat_dest = NULL, *prev;

	for (i = 0; i < num_channels; i++) {
		next_switch = sw->dij_channels[i];
		if (generate_routing_func_for_mst(p_lash, next_switch, &dest))
			return -1;

		i_dest = dest;
		prev = i_dest;

		while (i_dest != NULL) {
			if (sw->routing_table[i_dest->switch_id].out_link ==
			    NONE)
				sw->routing_table[i_dest->switch_id].out_link =
				    get_phys_connection(sw, next_switch);

			prev = i_dest;
			i_dest = i_dest->next;
		}

		CL_ASSERT(prev->next == NULL);
		prev->next = concat_dest;
		concat_dest = dest;
	}

	i_dest = (reachable_dest_t *) malloc(sizeof(reachable_dest_t));
	if (!i_dest)
		return -1;
	i_dest->switch_id = sw->id;
	i_dest->next = concat_dest;
	*destinations = i_dest;
	return 0;
}