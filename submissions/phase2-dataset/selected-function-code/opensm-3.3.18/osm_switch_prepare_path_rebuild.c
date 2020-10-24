int osm_switch_prepare_path_rebuild(IN osm_switch_t * p_sw, IN uint16_t max_lids)
{
	uint8_t **hops;
	unsigned i;

	if (alloc_lft(p_sw, max_lids))
		return -1;

	for (i = 0; i < p_sw->num_ports; i++)
		osm_port_prof_construct(&p_sw->p_prof[i]);

	osm_switch_clear_hops(p_sw);

	if (!(p_sw->new_lft = realloc(p_sw->new_lft, p_sw->lft_size)))
		return -1;

	memset(p_sw->new_lft, OSM_NO_PATH, p_sw->lft_size);

	if (!p_sw->hops) {
		hops = malloc((max_lids + 1) * sizeof(hops[0]));
		if (!hops)
			return -1;
		memset(hops, 0, (max_lids + 1) * sizeof(hops[0]));
		p_sw->hops = hops;
		p_sw->num_hops = max_lids + 1;
	} else if (max_lids + 1 > p_sw->num_hops) {
		hops = realloc(p_sw->hops, (max_lids + 1) * sizeof(hops[0]));
		if (!hops)
			return -1;
		memset(hops + p_sw->num_hops, 0,
		       (max_lids + 1 - p_sw->num_hops) * sizeof(hops[0]));
		p_sw->hops = hops;
		p_sw->num_hops = max_lids + 1;
	}
	p_sw->max_lid_ho = max_lids;

	return 0;
}