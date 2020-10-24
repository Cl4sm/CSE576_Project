static int
rlookup_binding(FILE *f, char *buff, char *map_alias)
{
	char line[LINE_MAX];
	unsigned int line_nr = 0;
	int id = 0;

	buff[0] = '\0';

	while (fgets(line, LINE_MAX, f)) {
		char *c, *alias, *wwid;
		int curr_id;

		line_nr++;
		c = strpbrk(line, "#\n\r");
		if (c)
			*c = '\0';
		alias = strtok(line, " \t");
		if (!alias) /* blank line */
			continue;
		curr_id = scan_devname(alias, NULL); /* TBD: Why this call? */
		if (curr_id >= id)
			id = curr_id + 1;
		wwid = strtok(NULL, " \t");
		if (!wwid){
			condlog(3,
				"Ignoring malformed line %u in bindings file",
				line_nr);
			continue;
		}
		if (strlen(wwid) > WWID_SIZE - 1) {
			condlog(3,
				"Ignoring too large wwid at %u in bindings file", line_nr);
			continue;
		}
		if (strcmp(alias, map_alias) == 0){
			condlog(3, "Found matching alias [%s] in bindings file."
				"\nSetting wwid to %s", alias, wwid);
			strncpy(buff, wwid, WWID_SIZE);
			buff[WWID_SIZE - 1] = '\0';
			return id;
		}
	}
	condlog(3, "No matching alias [%s] in bindings file.", map_alias);
	return id;
}
