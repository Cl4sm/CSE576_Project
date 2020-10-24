nn_map_t *open_node_name_map(const char *node_name_map)
{
	nn_map_t *map;
	char linebuf[PARSE_NODE_MAP_BUFLEN + 1];

	if (!node_name_map) {
#ifdef HAVE_DEFAULT_NODENAME_MAP
		struct stat buf;
		node_name_map = HAVE_DEFAULT_NODENAME_MAP;
		if (stat(node_name_map, &buf))
			return NULL;
#else
		return NULL;
#endif				/* HAVE_DEFAULT_NODENAME_MAP */
	}

	map = malloc(sizeof(*map));
	if (!map)
		return NULL;
	cl_qmap_init(map);

	memset(linebuf, '\0', PARSE_NODE_MAP_BUFLEN + 1);
	if (parse_node_map_wrap(node_name_map, map_name, map,
				linebuf, PARSE_NODE_MAP_BUFLEN)) {
		if (errno == EIO) {
			fprintf(stderr,
				"WARNING failed to parse node name map "
				"\"%s\"\n",
				node_name_map);
			fprintf(stderr,
				"WARNING failed line: \"%s\"\n",
				linebuf);
		}
		else
			fprintf(stderr,
				"WARNING failed to open node name map "
				"\"%s\" (%s)\n",
				node_name_map, strerror(errno));
		close_node_name_map(map);
		return NULL;
	}

	return map;
}