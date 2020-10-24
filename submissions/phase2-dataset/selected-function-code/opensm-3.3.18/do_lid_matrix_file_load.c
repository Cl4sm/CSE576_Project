static int do_lid_matrix_file_load(void *context)
{
	char line[1024];
	uint8_t hops[256];
	char *file_name;
	FILE *file;
	ib_net64_t guid;
	osm_opensm_t *p_osm = context;
	osm_switch_t *p_sw;
	unsigned lineno;
	uint16_t lid;
	int status = -1;

	file_name = p_osm->subn.opt.lid_matrix_dump_file;
	if (!file_name) {
		OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
			"lid matrix file name is not given; "
			"using default lid matrix generation algorithm\n");
		return 1;
	}

	file = fopen(file_name, "r");
	if (!file) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR | OSM_LOG_SYS, "ERR 6305: "
			"Can't open lid matrix file \'%s\': %m\n", file_name);
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

		if (!strncmp(p, "Switch", 6)) {
			q = strstr(p, " guid 0x");
			if (!q) {
				OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
					"PARSE ERROR: %s:%u: "
					"cannot parse switch definition\n",
					file_name, lineno);
				goto Exit;
			}
			p = q + 8;
			guid = strtoull(p, &q, 16);
			if (q == p || !isspace(*q)) {
				OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
					"PARSE ERROR: %s:%u: "
					"cannot parse switch guid: \'%s\'\n",
					file_name, lineno, p);
				goto Exit;
			}
			guid = cl_hton64(guid);

			p_sw = osm_get_switch_by_guid(&p_osm->subn, guid);
			if (!p_sw) {
				OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
					"cannot find switch %016" PRIx64 "\n",
					cl_ntoh64(guid));
				continue;
			}
		} else if (p_sw && !strncmp(p, "0x", 2)) {
			unsigned long num;
			unsigned len = 0;

			memset(hops, 0xff, sizeof(hops));

			p += 2;
			num = strtoul(p, &q, 16);
			if (num > 0xffff || q == p ||
			    (*q != ':' && !isspace(*q))) {
				OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
					"PARSE ERROR: %s:%u: "
					"cannot parse lid: \'%s\'\n",
					file_name, lineno, p);
				goto Exit;
			}
			/* Just checked the range, so casting is safe */
			lid = (uint16_t) num;
			p = q;
			while (isspace(*p) || *p == ':')
				p++;
			while (len < 256 && *p && *p != '#') {
				num = strtoul(p, &q, 16);
				if (num > 0xff || q == p) {
					OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
						"PARSE ERROR: %s:%u: "
						"cannot parse hops number: \'%s\'\n",
						file_name, lineno, p);
					goto Exit;
				}
				/* Just checked the range, so casting is safe */
				hops[len++] = (uint8_t) num;
				p = q;
				while (isspace(*p))
					p++;
			}
			/* additionally try to extract guid */
			q = strstr(p, " portguid 0x");
			if (!q) {
				OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
					"PARSE WARNING: %s:%u: "
					"cannot find port guid "
					"(maybe broken dump): \'%s\'\n",
					file_name, lineno, p);
				guid = 0;
			} else {
				p = q + 12;
				guid = strtoull(p, &q, 16);
				if (q == p || !isspace(*q)) {
					OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
						"PARSE WARNING: %s:%u: "
						"cannot parse port guid "
						"(maybe broken dump): \'%s\'\n",
						file_name, lineno, p);
					guid = 0;
				}
			}
			guid = cl_hton64(guid);
			add_lid_hops(p_osm, p_sw, lid, guid, hops, len);
		}
	}
	status = 0;
Exit:
	if (file)
		fclose(file);
	return status;
}