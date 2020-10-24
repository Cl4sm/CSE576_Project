static void _read_pin_file(const gchar *filename, GHashTable *pin_hash_table, gboolean first_run)
{
	g_assert(filename != NULL && strlen(filename) > 0);
	g_assert(pin_hash_table != NULL);

	GStatBuf sbuf;
	memset(&sbuf, 0, sizeof(sbuf));
	if (g_stat(filename, &sbuf) != 0) {
		if (first_run) {
			g_printerr("%s: %s\n", filename, strerror(errno));
			exit(EXIT_FAILURE);
		} else {
			return;
		}
	}
	if (!S_ISREG(sbuf.st_mode)) {
		if (first_run) {
			g_printerr("%s: It's not a regular file\n", filename);
			exit(EXIT_FAILURE);
		} else {
			return;
		}
	}
	if (sbuf.st_mode & S_IROTH) {
		if (first_run)
			g_print("Warning! %s is world readable!\n", filename);
	}

	FILE *fp = g_fopen(filename, "r");
	if (!fp) {
		if (first_run) {
			g_printerr("%s: %s\n", filename, strerror(errno));
			exit(EXIT_FAILURE);
		} else {
			return;
		}
	}

	g_hash_table_remove_all(pin_hash_table);

	gchar *line = NULL;
	size_t len = 0;
	ssize_t read;
	guint n = 0;
	GRegex *regex = g_regex_new("^(\\S+)\\s+(\\S+)$", 0, 0, NULL);

	while ((read = getline(&line, &len, fp)) != -1) {
		n++;

		if (g_regex_match_simple("^\\s*(#|$)", line, 0, 0))
			continue;

		GMatchInfo *match_info;
		if (g_regex_match(regex, line, 0, &match_info)) {
			gchar **t = g_match_info_fetch_all(match_info);
			/* Convert MAC to upper case */
			if (g_regex_match_simple("^([0-9a-fA-F]{2}(:|$)){6}$", t[1], 0, 0))
				g_hash_table_insert(pin_hash_table, g_ascii_strup(t[1], -1), g_strdup(t[2]));
			else
				g_hash_table_insert(pin_hash_table, g_strdup(t[1]), g_strdup(t[2]));
			g_strfreev(t);
		} else {
			if (first_run)
				g_print("%d: Invalid line (ignored)\n", n);
		}
		g_match_info_free(match_info);
	}

	g_regex_unref(regex);
	if (line)
		g_free(line);
	fclose(fp);

	first_run = FALSE;

	return;
}
