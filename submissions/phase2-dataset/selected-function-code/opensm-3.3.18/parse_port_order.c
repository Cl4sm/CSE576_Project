static
bool parse_port_order(struct torus *t, const char *parse_sep)
{
	unsigned i, j, k, n;

	for (i = 0; i < ARRAY_SIZE(t->port_order); i++) {
		if (!parse_unsigned(&(t->port_order[i]), parse_sep))
			break;

		for (j = 0; j < i; j++) {
			if (t->port_order[j] == t->port_order[i]) {
				OSM_LOG(&t->osm->log, OSM_LOG_INFO,
					"Ignored duplicate port %u in"
					" port_order parsing\n",
					t->port_order[j]);
				i--;	/* Ignore duplicate port number */
				break;
			}
		}
	}

	n = i;
	for (j = 0; j < ARRAY_SIZE(t->port_order); j++) {
		for (k = 0; k < i; k++)
			if (t->port_order[k] == j)
				break;
		if (k >= i)
			t->port_order[n++] = j;
	}

	return true;
}