static int set_search_ordering_ports(void *ctx, uint64_t guid, char *p)
{
	osm_subn_t *p_subn = ctx;
	osm_node_t *node = osm_get_node_by_guid(p_subn, cl_hton64(guid));
	osm_switch_t *sw;
	uint8_t *search_ordering_ports = NULL;
	uint8_t port;
	unsigned int *ports = NULL;
	const int bpw = sizeof(*ports)*8;
	int words;
	int i = 1; /* port 0 maps to port 0 */

	if (!node || !(sw = node->sw)) {
		OSM_LOG(&p_subn->p_osm->log, OSM_LOG_VERBOSE,
			"switch with guid 0x%016" PRIx64 " is not found\n",
			guid);
		return 0;
	}

	if (sw->search_ordering_ports) {
		OSM_LOG(&p_subn->p_osm->log, OSM_LOG_VERBOSE,
			"switch with guid 0x%016" PRIx64 " already listed\n",
			guid);
		return 0;
	}

	search_ordering_ports = malloc(sizeof(*search_ordering_ports)*sw->num_ports);
	if (!search_ordering_ports) {
		OSM_LOG(&p_subn->p_osm->log, OSM_LOG_ERROR,
			"ERR 3A07: cannot allocate memory for search_ordering_ports\n");
		return -1;
	}
	memset(search_ordering_ports, 0, sizeof(*search_ordering_ports)*sw->num_ports);

	/* the ports array is for record keeping of which ports have
	 * been seen */
	words = (sw->num_ports + bpw - 1)/bpw;
	ports = malloc(words*sizeof(*ports));
	if (!ports) {
		OSM_LOG(&p_subn->p_osm->log, OSM_LOG_ERROR,
			"ERR 3A08: cannot allocate memory for ports\n");
		free(search_ordering_ports);
		return -1;
	}
	memset(ports, 0, words*sizeof(*ports));

	while ((*p != '\0') && (*p != '#')) {
		char *e;

		port = strtoul(p, &e, 0);
		if ((p == e) || (port == 0) || (port >= sw->num_ports) ||
		    !osm_node_get_physp_ptr(node, port)) {
			OSM_LOG(&p_subn->p_osm->log, OSM_LOG_VERBOSE,
				"bad port %d specified for guid 0x%016" PRIx64 "\n",
				port, guid);
			free(search_ordering_ports);
			free(ports);
			return 0;
		}

		if (ports[port/bpw] & (1u << (port%bpw))) {
			OSM_LOG(&p_subn->p_osm->log, OSM_LOG_VERBOSE,
				"port %d already specified for guid 0x%016" PRIx64 "\n",
				port, guid);
			free(search_ordering_ports);
			free(ports);
			return 0;
		}

		ports[port/bpw] |= (1u << (port%bpw));
		search_ordering_ports[i++] = port;

		p = e;
		while (isspace(*p)) {
			p++;
		}
	}

	if (i > 1) {
		for (port = 1; port < sw->num_ports; port++) {
			/* fill out the rest of the search_ordering_ports array
			 * in sequence using the remaining unspecified
			 * ports.
			 */
			if (!(ports[port/bpw] & (1u << (port%bpw)))) {
				search_ordering_ports[i++] = port;
			}
		}
		sw->search_ordering_ports = search_ordering_ports;
	} else {
		free(search_ordering_ports);
	}

	free(ports);
	return 0;
}