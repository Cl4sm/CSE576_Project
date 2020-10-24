static int cycle_exists(cdg_vertex_t * start, cdg_vertex_t * current,
			cdg_vertex_t * prev, int visit_num)
{
	int i, new_visit_num;
	int cycle_found = 0;

	if (current != NULL && current->visiting_number > 0) {
		if (visit_num > current->visiting_number && current->seen == 0) {
			cycle_found = 1;
		}
	} else {
		if (current == NULL) {
			current = start;
			CL_ASSERT(prev == NULL);
		}

		current->visiting_number = visit_num;

		if (prev != NULL) {
			prev->next = current;
			CL_ASSERT(prev->to == current->from);
			CL_ASSERT(prev->visiting_number > 0);
		}

		new_visit_num = visit_num + 1;

		for (i = 0; i < current->num_deps; i++) {
			cycle_found =
			    cycle_exists(start, current->deps[i].v, current,
					 new_visit_num);
			if (cycle_found == 1)
				i = current->num_deps;
		}

		current->seen = 1;
		if (prev != NULL)
			prev->next = NULL;
	}

	return cycle_found;
}