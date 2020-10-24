void
rxvt_extract_resources (
	rxvt_t	    *r,
	Display	    *display __attribute__((unused)),
	const char  *name )
{
#ifndef NO_RESOURCES
    /* get resources the hard way, but save lots of memory */
    FILE	   *fd = NULL;
    char	   *home;

    macro_priority_t    priority = 1;   /* As resources get more general, we
					   increment this value so that
					   resources with lower priority are not
					   overwritten */

# if defined XAPPLOADDIR
#  if defined(HAVE_XSETLOCALE) || defined(HAVE_SETLOCALE)
    /* Compute the path of the possibly available localized Rxvt file */ 
    char	   *localepath = NULL;

    if (NOT_NULL(r->h->locale))
    {
	/* XXX: must limit length of string */

	localepath = rxvt_malloc(256); 
	sprintf( localepath, XAPPLOADDIRLOCALE "/" APL_SUBCLASS,
	    (int) ( 258 - sizeof(XAPPLOADDIRLOCALE) - sizeof(APL_SUBCLASS)),
	    r->h->locale);  /* 258 = 255 + 4 (-.*s) - 1 (/) */
    }
#  endif
# endif /* XAPPLOADDIR */


    /* open user supplied config file first */
    if (NOT_NULL(r->h->rs[Rs_confFile]))
	fd = fopen( r->h->rs[Rs_confFile], "r" );

    if (IS_NULL(fd) && NOT_NULL(home = getenv("HOME")))
    {
	int	i, len = STRLEN(home) + 2;
	char*	f = NULL;

	/* possible integer overflow? */
	assert (len > 0);
	for( i = 0; i < (sizeof(xnames) / sizeof(xnames[0])); i++ )
	{
	    /* possible integer overflow? */
	    assert (len + STRLEN(xnames[i]) >= len);
	    f = rxvt_realloc( f, ( len + STRLEN(xnames[i]) ) * sizeof(char) );

	    sprintf( f, "%s/%s", home, xnames[i] );

	    if (NOT_NULL(fd = fopen(f, "r")))
		break;
	}
	rxvt_free(f);
    }

    /*
     * The normal order to match resources is the following:
     *
     *	    @ global resources (partial match, ~/.Xdefaults)
     * 	    @ application file resources (XAPPLOADDIR/Rxvt)
     * 	    @ class resources (~/.Xdefaults)
     * 	    @ private resources (~/.Xdefaults)
     *
     * However, for the hand-rolled resources, the matching algorithm checks if
     * a resource string value has already been allocated and won't overwrite it
     * with (in this case) a less specific resource value.
     *
     * This avoids multiple allocation.  Also, when we've called this routine
     * command-line string options have already been applied so we needn't to
     * allocate for those resources.
     *
     * So, search in resources from most to least specific.
     *
     * Also, use a special sub-class so that we can use either or both of
     * "XTerm" and "Rxvt" as class names.
     */

    rxvt_get_xdefaults(r, fd, name, priority++);
    rxvt_get_xdefaults(r, fd, APL_SUBCLASS, priority++);
    rxvt_get_xdefaults(r, fd, APL_CLASS, priority++);
#if 0
    rxvt_get_xdefaults(r, fd, "", priority++);  /* partial match */
#endif
    if (NOT_NULL(fd))
	fclose(fd);

#  if defined(XAPPLOADDIR) && defined(USE_XAPPLOADDIR)
    {
	FILE*	ad = NULL;

#   if defined(HAVE_XSETLOCALE) || defined(HAVE_SETLOCALE)
	if (IS_NULL(localepath) || IS_NULL(ad = fopen(localepath, "r")))
#   endif
	    ad = fopen(XAPPLOADDIR "/" APL_SUBCLASS, "r");
	if (NOT_NULL(ad))
	{
	    rxvt_get_xdefaults(r, ad, APL_SUBCLASS, priority++);
#if 0
	    /*
	     * 2006-05-23 gi1242: If we don't use the X resource database, we
	     * don't have to match the empty class.
	     */
	    rxvt_get_xdefaults(r, ad, "", priority++);
#endif
	    fclose(ad);
	}
    }
#  endif	    /* XAPPLOADDIR */


    /*
     * Now read config from system wide config file.
     */
    if (NOTSET_OPTION(r, Opt2_noSysConfig) &&
        NOT_NULL(fd = fopen( PKG_CONF_DIR "/mrxvtrc", "r")))
    {
	rxvt_get_xdefaults( r, fd, APL_SUBCLASS, priority++ );
	fclose(fd);
    }

    /*
     * Unset resources that point to emptyResource
     *
     * 2006-06-02 gi1242: On second thought, let's leave them pointing to
     * emptyResource. That way we will be able to tell which resources have been
     * cleared by the user, and which were never set in the first place.
     */
#if 0
    {
	int i;

	for( i=0; i < NUM_RESOURCES; i++)
	{
	    if( r->h->rs[i] == emptyResource )
	    {
		rxvt_dbgmsg ((DBG_DEBUG, DBG_RESOURCE, "Setting resource #%d to NULL\n", i));
		r->h->rs[i] = NULL;
	    }
	}
    }
#endif

# if defined XAPPLOADDIR
#  if defined(HAVE_XSETLOCALE) || defined(HAVE_SETLOCALE)
    /* Free the path of the possibly available localized Rxvt file */ 
    rxvt_free(localepath);
#  endif
# endif

#endif		    /* NO_RESOURCES */


    /*
     * Clear the boolean and reverse flags from Options and Options2. Otherwise
     * this will cause trouble when we want to save the options. In that case,
     * the boolean flag is set for each boolean options. Then if we compare
     * Options(2) to the flag, we always get TRUE!
     */
    UNSET_OPTION(r, ( Opt_Reverse | IS_OPTION1 ) );
    UNSET_OPTION(r, ( Opt_Reverse | IS_OPTION2 ) );
    UNSET_OPTION(r, ( Opt_Reverse | IS_OPTION3 ) );
    UNSET_OPTION(r, ( Opt_Reverse | IS_OPTION4 ) );


    /*
     * even without resources, at least do this setup for command-line options
     * and command-line long options
     */
#ifdef MULTICHAR_SET
    if (r->h->rs[Rs_multichar_encoding])
	rxvt_set_multichar_encoding(r, r->h->rs[Rs_multichar_encoding]);
    else
    {
	char*    enc;

	if (NOT_NULL(enc = rxvt_get_encoding_from_locale (r)))
	    rxvt_set_multichar_encoding (r, enc);
	else
	    rxvt_set_multichar_encoding(r, MULTICHAR_ENCODING);
    }
#endif

#ifdef GREEK_SUPPORT
    /* this could be a function in grkelot.c */
    /* void set_greek_keyboard (const char * str); */
    if (r->h->rs[Rs_greek_keyboard])
    {
	if (!STRCMP(r->h->rs[Rs_greek_keyboard], "iso"))
	    greek_setmode(GREEK_ELOT928);   /* former -grk9 */
	else if (!STRCMP(r->h->rs[Rs_greek_keyboard], "ibm"))
	    greek_setmode(GREEK_IBM437);    /* former -grk4 */
    }
    {
	KeySym	      sym;

	if (r->h->rs[Rs_greektoggle_key] &&
	    ((sym = XStringToKeysym(r->h->rs[Rs_greektoggle_key]))!=0))
	    r->h->ks_greekmodeswith = sym;
    }
#endif		    /* GREEK_SUPPORT */
}