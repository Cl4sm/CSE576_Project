static void set_next_srcdest_pair(cdg_link_t * link, uint32_t srcdest)
{
	uint32_t new_size = 0, start_size = 2;
	uint32_t *tmp = NULL, *tmp2 = NULL;

	if (link->num_pairs == 0) {
		link->srcdest_pairs =
		    (uint32_t *) malloc(start_size * sizeof(uint32_t));
		link->srcdest_pairs[link->num_pairs] = srcdest;
		link->max_len = start_size;
		link->removed = 0;
	} else if (link->num_pairs == link->max_len) {
		new_size = link->max_len << 1;
		tmp = (uint32_t *) malloc(new_size * sizeof(uint32_t));
		tmp =
		    memcpy(tmp, link->srcdest_pairs,
			   link->max_len * sizeof(uint32_t));
		tmp2 = link->srcdest_pairs;
		link->srcdest_pairs = tmp;
		link->srcdest_pairs[link->num_pairs] = srcdest;
		free(tmp2);
		link->max_len = new_size;
	} else {
		link->srcdest_pairs[link->num_pairs] = srcdest;
	}
	link->num_pairs++;
}