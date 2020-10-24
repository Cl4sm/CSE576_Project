static void remove_temp_depend_for_sp(lash_t * p_lash, int sw, int dest_switch,
				      int lane)
{
	switch_t **switches = p_lash->switches;
	cdg_vertex_t ****cdg_vertex_matrix = p_lash->cdg_vertex_matrix;
	int next_switch, output_link, i;
	cdg_vertex_t *v;

	output_link = switches[sw]->routing_table[dest_switch].out_link;
	next_switch = get_next_switch(p_lash, sw, output_link);

	while (sw != dest_switch) {
		v = cdg_vertex_matrix[lane][sw][next_switch];
		CL_ASSERT(v != NULL);

		if (v->temp == 1) {
			cdg_vertex_matrix[lane][sw][next_switch] = NULL;
			free(v);
		} else {
			CL_ASSERT(v->num_temp_depend <= v->num_deps);
			v->num_deps = v->num_deps - v->num_temp_depend;
			v->num_temp_depend = 0;
			v->num_using_vertex--;

			for (i = v->num_deps; i < p_lash->num_switches - 1; i++)
				v->deps[i].num_used = 0;
		}

		sw = next_switch;
		output_link = switches[sw]->routing_table[dest_switch].out_link;

		if (sw != dest_switch)
			next_switch = get_next_switch(p_lash, sw, output_link);

	}
}