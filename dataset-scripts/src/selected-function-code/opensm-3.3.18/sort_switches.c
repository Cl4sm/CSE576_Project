static void sort_switches(lash_t *p_lash, mesh_t *mesh)
{
	unsigned int i, j;
	unsigned int num_switches = p_lash->num_switches;
	comp_t *comp;
	int *reverse;
	switch_t *s;
	switch_t **switches;

	comp = malloc(num_switches * sizeof(comp_t));
	reverse = malloc(num_switches * sizeof(int));
	switches = malloc(num_switches * sizeof(switch_t *));
	if (!comp || !reverse || !switches) {
		OSM_LOG(&p_lash->p_osm->log, OSM_LOG_ERROR,
			"Failed memory allocation - switches not sorted!\n");
		goto Exit;
	}

	for (i = 0; i < num_switches; i++) {
		comp[i].index = i;
		comp[i].ctx.mesh = mesh;
		comp[i].ctx.p_lash = p_lash;
	}

	qsort(comp, num_switches, sizeof(comp_t), compare_switches);

	for (i = 0; i < num_switches; i++)
		reverse[comp[i].index] = i;

	for (i = 0; i < num_switches; i++) {
		s = p_lash->switches[comp[i].index];
		switches[i] = s;
		s->id = i;
		for (j = 0; j < s->node->num_links; j++)
			s->node->links[j]->switch_id =
				reverse[s->node->links[j]->switch_id];
	}

	for (i = 0; i < num_switches; i++)
		p_lash->switches[i] = switches[i];

Exit:
	if (switches)
		free(switches);
	if (comp)
		free(comp);
	if (reverse)
		free(reverse);
}