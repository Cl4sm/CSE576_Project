static int
lookup_binding(FILE *f, char *map_wwid, char **map_alias, char *prefix)
{
	char buf[LINE_MAX];
	unsigned int line_nr = 0;
	int id = 1;
	int biggest_id = 1;
	int smallest_bigger_id = INT_MAX;

	*map_alias = NULL;

	while (fgets(buf, LINE_MAX, f)) {
		char *c, *alias, *wwid;
		int curr_id;

		line_nr++;
		c = strpbrk(buf, "#\n\r");
		if (c)
			*c = '\0';
		alias = strtok(buf, " \t");
		if (!alias) /* blank line */
			continue;
		curr_id = scan_devname(alias, prefix);
		if (curr_id == id)
			id++;
		if (curr_id > biggest_id)
			biggest_id = curr_id;
		if (curr_id > id && curr_id < smallest_bigger_id)
			smallest_bigger_id = curr_id;
		wwid = strtok(NULL, " \t");
		if (!wwid){
			condlog(3,
				"Ignoring malformed line %u in bindings file",
				line_nr);
			continue;
		}
		if (strcmp(wwid, map_wwid) == 0){
			condlog(3, "Found matching wwid [%s] in bindings file."
				" Setting alias to %s", wwid, alias);
			*map_alias = strdup(alias);
			if (*map_alias == NULL)
				condlog(0, "Cannot copy alias from bindings "
					"file : %s", strerror(errno));
			return 0;
		}
	}
	condlog(3, "No matching wwid [%s] in bindings file.", map_wwid);
	if (id < 0) {
		condlog(0, "no more available user_friendly_names");
		return 0;
	}
	if (id < smallest_bigger_id)
		return id;
	return biggest_id + 1;
}
