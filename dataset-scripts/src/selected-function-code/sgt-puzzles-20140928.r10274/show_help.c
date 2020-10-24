static void show_help(frontend *fe, const char *topic)
{
    const char *list = HELP_BROWSER_PATH;
    char path[PATH_MAX + 1];
    struct {
	const char *s;
	int len;
    } lang[3];
    int i;

    /*
     * Search for help file, trying:
     * 1. Version for this locale, ignoring encoding (HTML browsers
     *    must handle multiple encodings)
     * 2. Version for this locale, ignoring encoding and country
     * 3. English version
     */
    lang[0].s = setlocale(LC_MESSAGES, NULL);
    lang[0].len = strcspn(lang[0].s, ".@");
    lang[1].s = lang[0].s;
    lang[1].len = strcspn(lang[1].s, "_");
    if (lang[1].len > lang[0].len)
	lang[1].len = lang[0].len;
    lang[2].s = "en";
    lang[2].len = 2;
    for (i = 0; i < lenof(lang); i++) {
	sprintf(path, "%s/sgt-puzzles/help/%.*s/%s.html",
		SHAREDIR, lang[i].len, lang[i].s, topic);
	if (access(path, R_OK) == 0)
	    break;
    }
    if (i == lenof(lang)) {
	error_box(fe->window, "Help file is not installed");
	return;
    }

    for (;;) {
	size_t len;
	char buf[PATH_MAX + 1];
	const char *command;
	const char *argv[3];

	len = strcspn(list, ":");
	if (len <= PATH_MAX) {
	    memcpy(buf, list, len);
	    buf[len] = 0;
	    if (buf[0] == '$')
		command = getenv(buf + 1);
	    else
		command = buf;
	    if (command) {
		argv[0] = command;
		argv[1] = path;
		argv[2] = NULL;
		if (g_spawn_async(NULL, (char **)argv, NULL,
				  G_SPAWN_SEARCH_PATH,
				  NULL, NULL, NULL, NULL))
		    return;
	    }
	}

	if (!list[len])
	    break;
	list += len + 1;
    }

    error_box(fe->window, "Failed to start a help browser");
}