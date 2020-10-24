static void init_help(void)
{
#ifndef _WIN32_WCE
    char b[2048], *p, *q, *r;
    FILE *fp;

    /*
     * Find the executable file path, so we can look alongside
     * it for help files. Trim the filename off the end.
     */
    GetModuleFileName(NULL, b, sizeof(b) - 1);
    r = b;
    p = strrchr(b, '\\');
    if (p && p >= r) r = p+1;
    q = strrchr(b, ':');
    if (q && q >= r) r = q+1;

#ifndef NO_HTMLHELP
    /*
     * Try HTML Help first.
     */
    strcpy(r, CHM_FILE_NAME);
    if ( (fp = fopen(b, "r")) != NULL) {
	fclose(fp);

	/*
	 * We have a .CHM. See if we can use it.
	 */
	hh_dll = LoadLibrary("hhctrl.ocx");
	if (hh_dll) {
	    htmlhelp = (htmlhelp_t)GetProcAddress(hh_dll, "HtmlHelpA");
	    if (!htmlhelp)
		FreeLibrary(hh_dll);
	}
	if (htmlhelp) {
	    help_path = dupstr(b);
	    help_type = CHM;
	    return;
	}
    }
#endif /* NO_HTMLHELP */

    /*
     * Now try old-style .HLP.
     */
    strcpy(r, HELP_FILE_NAME);
    if ( (fp = fopen(b, "r")) != NULL) {
	fclose(fp);

	help_path = dupstr(b);
	help_type = HLP;

	/*
	 * See if there's a .CNT file alongside it.
	 */
	strcpy(r, HELP_CNT_NAME);
	if ( (fp = fopen(b, "r")) != NULL) {
	    fclose(fp);
	    help_has_contents = TRUE;
	} else
	    help_has_contents = FALSE;

	return;
    }

    help_type = NONE;	       /* didn't find any */
#endif
}