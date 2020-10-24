void
extract_resources(Display * display, const char *name)
{
#ifndef NO_RESOURCES
# ifdef USE_XGETDEFAULT
/*
 * get resources using the X library function
 */
    int             entry;

#  ifdef XrmEnumOneLevel
    XrmName         name_prefix[3];
    XrmClass        class_prefix[3];
    char           *displayResource;
    XrmDatabase     database;
    char           *screenResource;
    XrmDatabase     screenDatabase;

/*
 * Get screen-specific resources (X11R5) and merge into common resources.
 */
    database = NULL;
    screenDatabase = NULL;
    displayResource = XResourceManagerString(display);
    if (displayResource != NULL)
	database = XrmGetStringDatabase(displayResource);
    screenResource = XScreenResourceString(DefaultScreenOfDisplay(display));
    if (screenResource != NULL)
	screenDatabase = XrmGetStringDatabase(screenResource);
    XrmMergeDatabases(screenDatabase, &database);
    XrmSetDatabase(display, database);
#  endif

    for (entry = 0; entry < optList_size(); entry++) {
	int		s;
	char           *p, *p0;
	const char     *kw = optList[entry].kw;

	if (kw == NULL || *(optList[entry].dp) != NULL)
	    continue;		/* previously set */

#define STRCMP(x, y)		strcmp((const char *)(x), (const char *)(y))
	p = XGetDefault(display, name, kw);
	p0 = XGetDefault(display, "!INVALIDPROGRAMMENAMEDONTMATCH!", kw);
	if (p == NULL || (p0 && STRCMP(p, p0) == 0)) {
	    p = XGetDefault(display, APL_SUBCLASS, kw);
	    if (p == NULL || (p0 && STRCMP(p, p0) == 0))
		p = XGetDefault(display, APL_CLASS, kw);
	}
	if (p == NULL && p0)
	    p = p0;
	if (p) {
	    *optList[entry].dp = p;

	    if (optList_isBool(entry)) {
		s = my_strcasecmp(p, "TRUE") == 0
		    || my_strcasecmp(p, "YES") == 0
		    || my_strcasecmp(p, "ON") == 0
		    || my_strcasecmp(p, "1") == 0;
		if (optList_isReverse(entry))
		    s = !s;
		if (s)
		    Options |= (optList[entry].flag);
		else {
		    if (my_strcasecmp(p, "FALSE"))
			print_error("Cannot parse value \"%s\" from resource \"%s\" as boolean",
				    p, kw);
		    Options &= ~(optList[entry].flag);
		}
	    }
	}
    }

/*
 * [R5 or later]: enumerate the resource database
 */
#  ifdef XrmEnumOneLevel
#   ifdef KEYSYM_RESOURCE
    name_prefix[0] = XrmStringToName(name);
    name_prefix[1] = XrmStringToName("keysym");
    name_prefix[2] = NULLQUARK;
    class_prefix[0] = XrmStringToName(APL_SUBCLASS);
    class_prefix[1] = XrmStringToName("Keysym");
    class_prefix[2] = NULLQUARK;
    XrmEnumerateDatabase(XrmGetDatabase(display),
			 name_prefix,
			 class_prefix,
			 XrmEnumOneLevel,
			 define_key,
			 NULL);
    name_prefix[0] = XrmStringToName(APL_CLASS);
    name_prefix[1] = XrmStringToName("keysym");
    class_prefix[0] = XrmStringToName(APL_CLASS);
    class_prefix[1] = XrmStringToName("Keysym");
    XrmEnumerateDatabase(XrmGetDatabase(display),
			 name_prefix,
			 class_prefix,
			 XrmEnumOneLevel,
			 define_key,
			 NULL);
#   endif
#  endif

# else				/* USE_XGETDEFAULT */
/* get resources the hard way, but save lots of memory */
    const char     *fname[] = { ".Xdefaults", ".Xresources" };
    FILE           *fd = NULL;
    char           *home;

    if ((home = getenv("HOME")) != NULL) {
	int             i, len = strlen(home) + 2;
	char           *f = NULL;

	for (i = 0; i < (sizeof(fname) / sizeof(fname[0])); i++) {
	    f = REALLOC(f, (len + strlen(fname[i])) * sizeof(char));

	    sprintf(f, "%s/%s", home, fname[i]);

	    if ((fd = fopen(f, "r")) != NULL)
		break;
	}
	FREE(f);
    }
/*
 * The normal order to match resources is the following:
 * @ global resources (partial match, ~/.Xdefaults)
 * @ application file resources (XAPPLOADDIR/Rxvt)
 * @ class resources (~/.Xdefaults)
 * @ private resources (~/.Xdefaults)
 *
 * However, for the hand-rolled resources, the matching algorithm
 * checks if a resource string value has already been allocated
 * and won't overwrite it with (in this case) a less specific
 * resource value.
 *
 * This avoids multiple allocation.  Also, when we've called this
 * routine command-line string options have already been applied so we
 * needn't to allocate for those resources.
 *
 * So, search in resources from most to least specific.
 *
 * Also, use a special sub-class so that we can use either or both of
 * "XTerm" and "Rxvt" as class names.
 */

    get_xdefaults(fd, name);
    get_xdefaults(fd, APL_SUBCLASS);

#  ifdef XAPPLOADDIR
    {
	FILE           *ad = fopen(XAPPLOADDIR "/" APL_SUBCLASS, "r");

	if (ad != NULL) {
	    get_xdefaults(ad, "");
	    fclose(ad);
	}
    }
#  endif				/* XAPPLOADDIR */

    get_xdefaults(fd, APL_CLASS);
    get_xdefaults(fd, "");	/* partial match */
    if (fd != NULL)
	fclose(fd);
# endif				/* USE_XGETDEFAULT */
#endif				/* NO_RESOURCES */

/*
 * even without resources, at least do this setup for command-line
 * options and command-line long options
 */
#ifdef MULTICHAR_SET
    set_multichar_encoding(rs_multichar_encoding);
#endif
#ifdef GREEK_SUPPORT
/* this could be a function in grkelot.c */
/* void set_greek_keyboard (const char * str); */
    if (rs_greek_keyboard) {
	if (!strcmp(rs_greek_keyboard, "iso"))
	    greek_setmode(GREEK_ELOT928);	/* former -grk9 */
	else if (!strcmp(rs_greek_keyboard, "ibm"))
	    greek_setmode(GREEK_IBM437);	/* former -grk4 */
    }
#endif				/* GREEK_SUPPORT */

#define to_keysym(pks,str) do { KeySym sym;\
if (str && ((sym = XStringToKeysym(str)) != 0)) *pks = sym; } while (0)

#if defined (HOTKEY_CTRL) || defined (HOTKEY_META)
    to_keysym(&ks_bigfont, rs_bigfont_key);
    to_keysym(&ks_smallfont, rs_smallfont_key);
#endif
#undef to_keysym
}
