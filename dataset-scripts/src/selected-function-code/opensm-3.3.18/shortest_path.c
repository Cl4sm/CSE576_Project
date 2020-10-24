static void shortest_path(lash_t * p_lash, int ir)
{
	switch_t **switches = p_lash->switches, *sw, *swi;
	unsigned int i;
	cl_list_t bfsq;

	cl_list_construct(&bfsq);
	cl_list_init(&bfsq, 20);

	enqueue(&bfsq, switches[ir]);

	while (!cl_is_list_empty(&bfsq)) {
		dequeue(&bfsq, &sw);
		for (i = 0; i < sw->node->num_links; i++) {
			swi = switches[sw->node->links[i]->switch_id];
			if (swi->q_state == UNQUEUED) {
				enqueue(&bfsq, swi);
				sw->dij_channels[sw->used_channels++] = swi->id;
			}
		}
	}

	cl_list_destroy(&bfsq);
}