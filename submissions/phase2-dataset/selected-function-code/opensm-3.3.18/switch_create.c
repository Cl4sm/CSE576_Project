static switch_t *switch_create(lash_t * p_lash, unsigned id, osm_switch_t * p_sw)
{
	unsigned num_switches = p_lash->num_switches;
	unsigned num_ports = p_sw->num_ports;
	switch_t *sw;
	unsigned int i;

	sw = malloc(sizeof(*sw) + num_switches * sizeof(sw->routing_table[0]));
	if (!sw)
		return NULL;

	memset(sw, 0, sizeof(*sw));
	for (i = 0; i < num_switches; i++) {
		sw->routing_table[i].out_link = NONE;
		sw->routing_table[i].lane = NONE;
	}

	sw->id = id;
	sw->dij_channels = malloc(num_ports * sizeof(int));
	if (!sw->dij_channels) {
		free(sw);
		return NULL;
	}

	sw->p_sw = p_sw;
	p_sw->priv = sw;

	if (osm_mesh_node_create(p_lash, sw)) {
		free(sw->dij_channels);
		free(sw);
		return NULL;
	}

	return sw;
}