static int do_ucast_file_load(void *context)
{
	char line[1024];
	char *file_name;
	FILE *file;
	ib_net64_t sw_guid, port_guid;
	osm_opensm_t *p_osm = context;
	osm_switch_t *p_sw;
	uint16_t lid;
	uint8_t port_num;
	unsigned lineno;
	int status = -1;

	file_name = p_osm->subn.opt.lfts_file;
	if (!file_name) {
		OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
			"LFTs file name is not given; "
			"using default routing algorithm\n");
		return 1;
	}

	file = fopen(file_name, "r");
	if (!file) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR | OSM_LOG_SYS, "ERR 6302: "
			"Can't open ucast dump file \'%s\': %m\n", file_name);
		goto Exit;
	}

	lineno = 0;
	p_sw = NULL;

	while (fgets(line, sizeof(line) - 1, file) != NULL) {
		char *p, *q;
		lineno++;

		p = line;
		while (isspace(*p))
			p++;

		if (*p == '#')
			continue;

		if (!strncmp(p, "Multicast mlids", 15)) {
			OSM_LOG(&p_osm->log, OSM_LOG_ERROR | OSM_LOG_SYS,
				"ERR 6303: "
				"Multicast dump file detected; "
				"skipping parsing. Using default "
				"routing algorithm\n");
		} else if (!strncmp(p, "Unicast lids", 12)) {
			q = strstr(p, " guid 0x");
			if (!q) {
				OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
					"PARSE ERROR: %s:%u: "
					"cannot parse switch definition\n",
					file_name, lineno);
				goto Exit;
			}
			p = q + 8;
			sw_guid = strtoull(p, &q, 16);
			if (q == p || !isspace(*q)) {
				OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
					"PARSE ERROR: %s:%u: "
					"cannot parse switch guid: \'%s\'\n",
					file_name, lineno, p);
				goto Exit;
			}
			sw_guid = cl_hton64(sw_guid);

			p_sw = osm_get_switch_by_guid(&p_osm->subn, sw_guid);
			if (!p_sw) {
				OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
					"cannot find switch %016" PRIx64 "\n",
					cl_ntoh64(sw_guid));
				continue;
			}
			memset(p_sw->new_lft, OSM_NO_PATH, p_sw->lft_size);
		} else if (p_sw && !strncmp(p, "0x", 2)) {
			p += 2;
			lid = (uint16_t) strtoul(p, &q, 16);
			if (q == p || !isspace(*q)) {
				OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
					"PARSE ERROR: %s:%u: "
					"cannot parse lid: \'%s\'\n",
					file_name, lineno, p);
				goto Exit;
			}
			p = q;
			while (isspace(*p))
				p++;
			port_num = (uint8_t) strtoul(p, &q, 10);
			if (q == p || !isspace(*q)) {
				OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
					"PARSE ERROR: %s:%u: "
					"cannot parse port: \'%s\'\n",
					file_name, lineno, p);
				goto Exit;
			}
			if (port_num >=
				osm_node_get_num_physp(p_sw->p_node)) {
					OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
						"Invalid port %d found "
						"for switch %016" PRIx64 "\n",
						port_num,
						cl_ntoh64(osm_node_get_node_guid
							(p_sw->p_node)));
					goto Exit;
			}

			p = q;
			/* additionally try to extract guid */
			q = strstr(p, " portguid 0x");
			if (!q) {
				OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
					"PARSE WARNING: %s:%u: "
					"cannot find port guid "
					"(maybe broken dump): \'%s\'\n",
					file_name, lineno, p);
				port_guid = 0;
			} else {
				p = q + 12;
				port_guid = strtoull(p, &q, 16);
				if (q == p || (!isspace(*q) && *q != ':')) {
					OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
						"PARSE WARNING: %s:%u: "
						"cannot parse port guid "
						"(maybe broken dump): \'%s\'\n",
						file_name, lineno, p);
					port_guid = 0;
				}
			}
			port_guid = cl_hton64(port_guid);
			add_path(p_osm, p_sw, lid, port_num, port_guid);
		}
	}
	status = 0;
Exit:
	if (file)
		fclose(file);
	return status;
}