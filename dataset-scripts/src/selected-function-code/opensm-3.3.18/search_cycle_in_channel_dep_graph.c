static cdg_node_t *search_cycle_in_channel_dep_graph(cdg_node_t * cdg,
						     cdg_node_t * start_node)
{
	cdg_node_t *cycle = NULL;
	cdg_node_t *current = start_node, *next_node = NULL, *tmp = NULL;
	cdg_link_t *link = NULL;

	while (current) {
		current->status = GRAY;
		link = current->linklist;
		next_node = NULL;
		while (link) {
			if (link->node->status == UNKNOWN) {
				next_node = link->node;
				break;
			}
			if (link->node->status == GRAY) {
				cycle = link->node;
				goto Exit;
			}
			link = link->next;
		}
		if (next_node) {
			next_node->pre = current;
			current = next_node;
		} else {
			/* found a sink in the graph, go to last node */
			current->status = BLACK;

			/* srcdest_pairs of this node aren't relevant, free the allocated memory */
			link = current->linklist;
			while (link) {
				if (link->num_pairs)
					free(link->srcdest_pairs);
				link->srcdest_pairs = NULL;
				link->num_pairs = 0;
				link->removed = 0;
				link = link->next;
			}

			if (current->pre) {
				tmp = current;
				current = current->pre;
				tmp->pre = NULL;
			} else {
				/* search for other subgraphs in cdg */
				current = get_next_cdg_node(cdg);
				if (!current)
					break;	/* all relevant nodes traversed, no more cycles found */
			}
		}
	}

Exit:
	return cycle;
}