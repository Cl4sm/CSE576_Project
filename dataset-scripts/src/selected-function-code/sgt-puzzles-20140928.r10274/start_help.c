static void start_help(frontend *fe, const char *topic)
{
    char *str = NULL;
    int cmd;

    switch (help_type) {
      case HLP:
	assert(help_path);
	if (topic) {
	    str = snewn(10+strlen(topic), char);
	    sprintf(str, "JI(`',`%s')", topic);
	    cmd = HELP_COMMAND;
	} else if (help_has_contents) {
	    cmd = HELP_FINDER;
	} else {
	    cmd = HELP_CONTENTS;
	}
	WinHelp(fe->hwnd, help_path, cmd, (DWORD)str);
	fe->help_running = TRUE;
	break;
      case CHM:
#ifndef NO_HTMLHELP
	assert(help_path);
	assert(htmlhelp);
	if (topic) {
	    str = snewn(20 + strlen(topic) + strlen(help_path), char);
	    sprintf(str, "%s::/%s.html>main", help_path, topic);
	} else {
	    str = dupstr(help_path);
	}
	htmlhelp(fe->hwnd, str, HH_DISPLAY_TOPIC, 0);
	fe->help_running = TRUE;
	break;
#endif /* NO_HTMLHELP */
      case NONE:
	assert(!"This shouldn't happen");
	break;
    }

    sfree(str);
}