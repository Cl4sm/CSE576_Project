void osm_switch_delete(IN OUT osm_switch_t ** pp_sw)
{
	osm_switch_t *p_sw = *pp_sw;
	unsigned i;

	osm_mcast_tbl_destroy(&p_sw->mcast_tbl);
	if (p_sw->p_prof)
		free(p_sw->p_prof);
	if (p_sw->search_ordering_ports)
		free(p_sw->search_ordering_ports);
	if (p_sw->lft)
		free(p_sw->lft);
	if (p_sw->new_lft)
		free(p_sw->new_lft);
	if (p_sw->hops) {
		for (i = 0; i < p_sw->num_hops; i++)
			if (p_sw->hops[i])
				free(p_sw->hops[i]);
		free(p_sw->hops);
	}
	free(*pp_sw);
	*pp_sw = NULL;
}