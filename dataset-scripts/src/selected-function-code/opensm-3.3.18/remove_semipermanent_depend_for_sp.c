static void remove_semipermanent_depend_for_sp(lash_t * p_lash, int sw,
					       int dest_switch, int lane)
{
	switch_t **switches = p_lash->switches;
	cdg_vertex_t ****cdg_vertex_matrix = p_lash->cdg_vertex_matrix;
	int i_next_switch, output_link, i, next_link, i_next_next_switch,
	    depend = 0;
	cdg_vertex_t *v;
	int __attribute__((unused)) found;

	output_link = switches[sw]->routing_table[dest_switch].out_link;
	i_next_switch = get_next_switch(p_lash, sw, output_link);

	while (sw != dest_switch) {
		v = cdg_vertex_matrix[lane][sw][i_next_switch];
		CL_ASSERT(v != NULL);

		if (v->num_using_vertex == 1) {

			cdg_vertex_matrix[lane][sw][i_next_switch] = NULL;

			free(v);
		} else {
			v->num_using_vertex--;
			if (i_next_switch != dest_switch) {
				next_link =
				    switches[i_next_switch]->routing_table[dest_switch].out_link;
				i_next_next_switch = get_next_switch(p_lash, i_next_switch, next_link);
				found = 0;

				for (i = 0; i < v->num_deps; i++)
					if (v->deps[i].v ==
					    cdg_vertex_matrix[lane][i_next_switch]
					    [i_next_next_switch]) {
						found = 1;
						depend = i;
					}

				CL_ASSERT(found);

				if (v->deps[depend].num_used == 1) {
					for (i = depend;
					     i < v->num_deps - 1; i++) {
						v->deps[i].v = v->deps[i + 1].v;
						v->deps[i].num_used =
						    v->deps[i + 1].num_used;
					}

					v->num_deps--;
				} else
					v->deps[depend].num_used--;
			}
		}

		sw = i_next_switch;
		output_link = switches[sw]->routing_table[dest_switch].out_link;

		if (sw != dest_switch)
			i_next_switch = get_next_switch(p_lash, sw, output_link);
	}
}