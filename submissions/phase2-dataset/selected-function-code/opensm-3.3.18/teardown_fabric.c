static
void teardown_fabric(struct fabric *f)
{
	unsigned l, p, s;
	struct endpoint *port;
	struct f_switch *sw;

	if (!f)
		return;

	if (f->sw) {
		/*
		 * Need to free switches, and also find/free the endpoints
		 * we allocated for switch management ports.
		 */
		for (s = 0; s < f->switch_cnt; s++) {
			sw = f->sw[s];
			if (!sw)
				continue;

			for (p = 0; p < sw->port_cnt; p++) {
				port = sw->port[p];
				if (port && !port->link)
					free(port);	/* management port */
			}
			free(sw);
		}
		free(f->sw);
	}
	if (f->link) {
		for (l = 0; l < f->link_cnt; l++)
			if (f->link[l])
				free(f->link[l]);

		free(f->link);
	}
	memset(f, 0, sizeof(*f));
}