void teardown_torus(struct torus *t)
{
	unsigned p, s;
	struct endpoint *port;
	struct t_switch *sw;

	if (!t)
		return;

	if (t->sw_pool) {
		/*
		 * Need to free switches, and also find/free the endpoints
		 * we allocated for switch management ports.
		 */
		for (s = 0; s < t->switch_cnt; s++) {
			sw = t->sw_pool[s];
			if (!sw)
				continue;

			for (p = 0; p < sw->port_cnt; p++) {
				port = sw->port[p];
				if (port && !port->link)
					free(port);	/* management port */
			}
			free(sw);
		}
		free(t->sw_pool);
	}
	if (t->link_pool)
		free(t->link_pool);

	if (t->sw)
		free(t->sw);

	if (t->seed)
		free(t->seed);

	free(t);
}