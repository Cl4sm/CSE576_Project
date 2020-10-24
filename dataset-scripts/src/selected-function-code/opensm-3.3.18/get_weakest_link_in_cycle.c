static cdg_link_t *get_weakest_link_in_cycle(cdg_node_t * cycle)
{
	cdg_node_t *current = cycle, *node_with_weakest_link = NULL;
	cdg_link_t *link = NULL, *weakest_link = NULL;

	link = current->linklist;
	while (link) {
		if (link->node->status == GRAY) {
			weakest_link = link;
			node_with_weakest_link = current;
			current = link->node;
			break;
		}
		link = link->next;
	}

	while (1) {
		current->status = UNKNOWN;
		link = current->linklist;
		while (link) {
			if (link->node->status == GRAY) {
				if ((link->num_pairs - link->removed) <
				    (weakest_link->num_pairs -
				     weakest_link->removed)) {
					weakest_link = link;
					node_with_weakest_link = current;
				}
				current = link->node;
				break;
			}
			link = link->next;
		}
		/* if complete cycle is traversed */
		if (current == cycle) {
			current->status = UNKNOWN;
			break;
		}
	}

	if (node_with_weakest_link->linklist == weakest_link) {
		node_with_weakest_link->linklist = weakest_link->next;
	} else {
		link = node_with_weakest_link->linklist;
		while (link) {
			if (link->next == weakest_link) {
				link->next = weakest_link->next;
				break;
			}
			link = link->next;
		}
	}

	return weakest_link;
}