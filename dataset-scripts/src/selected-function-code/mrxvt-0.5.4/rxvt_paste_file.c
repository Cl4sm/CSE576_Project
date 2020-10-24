 * EXT: button 2 release
 */
/* EXTPROTO */
void
rxvt_paste_file(rxvt_t* r, int page, Time tm, int x, int y, char* filename)
{
    rxvt_dbgmsg(( DBG_DEBUG, DBG_SCREEN,
		"rxvt_paste_file %d (%lu, %d, %d) %s\n", page, tm, x, y,
		filename ));

    if (x < 0 || x >= VT_WIDTH(r) || y < 0 || y >= VT_HEIGHT(r))
	return;		/* outside window */

    char buffer[BUFSIZ];
    char TAINTED * str;
    FILE * fdpaste;

#ifdef HAVE_WORDEXP_H
    wordexp_t p;
    int wordexp_result;

    /* perform a shell-like expansion of the provided filename */
    wordexp_result = wordexp(filename, &p, 0);
    if( wordexp_result == 0 && p.we_wordc == 1 )
	filename = *p.we_wordv;
    else
    {
	rxvt_msg( DBG_ERROR, DBG_SCREEN,
		    "Error expanding %s, or possibly ambiguous expansion\n",
		    filename );
	rxvt_msg( DBG_INFO, DBG_SCREEN, "wordexp_result=%i\n", wordexp_result );
    }
#endif

    if (NOT_NULL(fdpaste = fopen( filename , "r")))
    {
	while (NOT_NULL(str = fgets(buffer, sizeof(buffer), fdpaste)))
	    rxvt_paste_str( r, page, (const unsigned char*) str , STRLEN(str));

	fclose(fdpaste);
    }
    else
    {
	rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN,
		    "rxvt_paste_file : unable to open file '%s'\n", filename));
    }

#ifdef HAVE_WORDEXP_H
    wordfree(&p);