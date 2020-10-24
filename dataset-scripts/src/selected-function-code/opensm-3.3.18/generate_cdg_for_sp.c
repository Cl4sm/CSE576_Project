static int generate_cdg_for_sp(lash_t * p_lash, int sw, int dest_switch,
			       int lane)
{
	unsigned num_switches = p_lash->num_switches;
	switch_t **switches = p_lash->switches;
	cdg_vertex_t ****cdg_vertex_matrix = p_lash->cdg_vertex_matrix;
	int next_switch, output_link, j, exists;
	cdg_vertex_t *v, *prev = NULL;

	output_link = switches[sw]->routing_table[dest_switch].out_link;
	next_switch = get_next_switch(p_lash, sw, output_link);

	while (sw != dest_switch) {

		if (cdg_vertex_matrix[lane][sw][next_switch] == NULL) {
			v = calloc(1, sizeof(*v) + (num_switches - 1) * sizeof(v->deps[0]));
			if (!v)
				return -1;
			v->from = sw;
			v->to = next_switch;
			v->temp = 1;
			cdg_vertex_matrix[lane][sw][next_switch] = v;
		} else
			v = cdg_vertex_matrix[lane][sw][next_switch];

		v->num_using_vertex++;

		if (prev != NULL) {
			exists = 0;

			for (j = 0; j < prev->num_deps; j++)
				if (prev->deps[j].v == v) {
					exists = 1;
					prev->deps[j].num_used++;
				}

			if (exists == 0) {
				prev->deps[prev->num_deps].v = v;
				prev->deps[prev->num_deps].num_used++;
				prev->num_deps++;

				CL_ASSERT(prev->num_deps < (int)num_switches);

				if (prev->temp == 0)
					prev->num_temp_depend++;

			}
		}

		sw = next_switch;
		output_link = switches[sw]->routing_table[dest_switch].out_link;

		if (sw != dest_switch) {
			CL_ASSERT(output_link != NONE);
			next_switch = get_next_switch(p_lash, sw, output_link);
		}

		prev = v;
	}
	return 0;
}