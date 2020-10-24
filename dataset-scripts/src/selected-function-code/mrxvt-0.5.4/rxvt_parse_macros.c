int
rxvt_parse_macros( rxvt_t *r, const char *str, const char *arg,
	macro_priority_t priority)
{
    char	    newarg[NEWARGLIM],
		    keyname[ NEWARGLIM],
		    *keyname_nomods;
    unsigned char   modFlags = 0;
    KeySym	    keysym;

    Bool	    addmacro = False;

    if (IS_NULL(arg))
    {
	char *keyend;
	int	n;

	/*
	 * Need to split str into keyname and argument.
	 */
	if( (n = rxvt_str_match(str, "macro.")) == 0 )
	    return 0;
	str += n;	/* skip `macro.' */

	if (IS_NULL(keyend = STRCHR( str, ':' )))
	    return -1;

	n = min( keyend - str, NEWARGLIM - 1 );

	STRNCPY( keyname, str, n );
	keyname[n] = 0;

	STRNCPY( newarg, keyend + 1, NEWARGLIM - 1 );
    }
    else
    {
	/*
	 * Keyname is already in str. Copy arg into newarg.
	 */
	STRNCPY( keyname, str, NEWARGLIM - 1 );
	keyname[ NEWARGLIM - 1] = '\0';

	STRNCPY( newarg, arg, NEWARGLIM - 1 );
    }

    /* Null terminate and strip leading / trailing spaces */
    newarg[NEWARGLIM - 1] = '\0';
    rxvt_str_trim( newarg );

    rxvt_msg( DBG_INFO, DBG_MACROS,
	    "Got macro '%s' -- '%s'\n", keyname, newarg );

    /*
     * Breakup keyname into a keysym and modifier flags.
     */
    if (IS_NULL(keyname_nomods = STRRCHR( keyname, '+' )))
    {
	/* No modifiers specified */
#ifdef UNSHIFTED_MACROS
	keyname_nomods = keyname;
#else
	return -1;
#endif
    }
    else
    {
	*(keyname_nomods++) = 0;

	/*
	 * keyname is now a null terminated string containing only the
	 * modifiers, and keyname_nomods is a null terminated string
	 * containing only the key name.
	 */
	if( STRCASESTR( keyname, "ctrl" ) )
	    modFlags |= MACRO_CTRL;
	if( STRCASESTR( keyname, "meta" ) || STRCASESTR( keyname, "alt"))
	    modFlags |= MACRO_META;
	if( STRCASESTR( keyname, "shift") )
	    modFlags |= MACRO_SHIFT;
	if( STRCASESTR( keyname, "primary"))
	    modFlags |= MACRO_PRIMARY;
	if( STRCASESTR( keyname, "add" ) )
	    addmacro = True;
    }

    /*
     * Always store the keysym as upper case. That way we can treat shift
     * correctly even when Caps Lock is pressed.
     */
    {
        KeySym upper;
        XConvertCase(XStringToKeysym(keyname_nomods), &keysym, &upper);
    }

    if (NOT_KEYSYM(keysym))
    {
	rxvt_msg (DBG_ERROR, DBG_MACROS,  "Invalid keysym %s. Skipping macro.",
		keyname_nomods);
	return -1;
    }

    return rxvt_add_macro( r, keysym, modFlags, newarg, addmacro, priority)
		? 1 : -1;
}