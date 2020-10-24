int
rxvt_add_macro( rxvt_t *r, KeySym keysym, unsigned char modFlags, char *astring,
	Bool addmacro, macro_priority_t priority)
{
    const unsigned nmacros_increment = 64;  /* # extra macros to alloc space for
					       when we need to enlarge our list
					       of macros. A large number here is
					       not wasteful as we clean it up
					       after initialization */
    unsigned short  i,
		    replaceIndex = r->nmacros,
		    dummyIndex = r->nmacros;

    unsigned char   macroNum = 0;
    action_t	    action;


    rxvt_dbgmsg(( DBG_DEBUG, DBG_MACROS, "%s(%08lx, %2hhx, '%s', %d, %d)\n",
		__func__, keysym, modFlags, astring, addmacro, priority ));

    /*
     * Check to see if macro already exists.
     */
    for( i=0; i < r->nmacros; i++ )
    {
	if(
	    r->macros[i].keysym == keysym
	    && (r->macros[i].modFlags & MACRO_MODMASK & ~MACRO_PRIMARY)
		    == (modFlags & MACRO_MODMASK & ~MACRO_PRIMARY)
	  )
	{
	    if( addmacro && r->macros[i].priority == priority )
	    {
		/*
		 * Find the last macro in the macro chain (the macro with the
		 * largest number).
		 */
		unsigned char num;

		num	 = MACRO_GET_NUMBER( r->macros[i].modFlags );
		if( num > macroNum )
		    macroNum = num;

		if( macroNum == MACRO_MAX_CHAINLEN )
		{
		    rxvt_msg (DBG_ERROR, DBG_MACROS,  "Macro chain too long" );
		    return 0;
		}

		replaceIndex = i;
	    }

	    /*
	     * Macro for key already exists. Only replace it if we have a
	     * *lower* priority (which in theory should never happen).
	     */
	    else if( priority > r->macros[i].priority )
		return 1; /* Claim to have succeded so that caller will not
			     complain about "Failing to add a ... macro". */
	    
	    /*
	     * 2006-03-06 gi1242: Don't delete "Dummy" macros here. If we do
	     * that then the user will have no way to delete macros defined in
	     * the system /etc/mrxvt/mrxvtrc file. "Dummy" macros will be
	     * deleted after init.
	     */
	    else
	    {
		if( replaceIndex < r->nmacros )
		{
		    /*
		     * replaceIndex points to a macro with keysym == the keysym
		     * of the macro to be added. Set keysym to 0 so that it will
		     * be cleaned up by rxvt_cleanup_macros().
		     */
		    r->macros[replaceIndex].keysym = 0;
		}
		replaceIndex = i;
	    }
	}

	else if( r->macros[i].keysym == 0 )
	    /*
	     * Macros with keysym 0 are dummies, and can be safely replaced.
	     */
	    dummyIndex = i;
    } /* for */

    /*
     * Now dummyIndex will point to a macro that can safely be replaced.
     * replaceIndex (if less than r->nmacros) will be the index of a macro in
     * r->macros which has the same keysym / modflags as the macro we wish to
     * add. We will either replace this macro with the new one, or add to it if
     * the "Add" flag was specified.
     */
    if( addmacro )
    {
	if( replaceIndex == r->nmacros )
	{
	    rxvt_msg( DBG_ERROR, DBG_MACROS,
		    "No previous macro to add to (key %s%s%s%s)",
		    (modFlags & MACRO_CTRL) ? "Ctrl+" : "",
		    (modFlags & MACRO_META) ? "Meta+" : "",
		    (modFlags & MACRO_SHIFT) ? "Shift+" : "",
		    XKeysymToString( keysym ) );
	    return 0;	/* Failure */
	}

	else if( r->macros[replaceIndex].priority != priority )
	{
	    rxvt_msg( DBG_ERROR, DBG_MACROS,
		    "Can not add to a macro defined at a different location "
		    "(key %s%s%s%s)",
		    (modFlags & MACRO_CTRL) ? "Ctrl+" : "",
		    (modFlags & MACRO_META) ? "Meta+" : "",
		    (modFlags & MACRO_SHIFT) ? "Shift+" : "",
		    XKeysymToString( keysym ) );
	    return 0;	/* Failure */
	}

	else if( r->macros[replaceIndex].action.type == MacroFnDummy )
	{
	    /* Do not add to a dummy macro */
	    rxvt_msg (DBG_ERROR, DBG_MACROS,
		    "Can not add actions to a Dummy macro"
		    "(key %s%s%s%s)",
		    (modFlags & MACRO_CTRL) ? "Ctrl+" : "",
		    (modFlags & MACRO_META) ? "Meta+" : "",
		    (modFlags & MACRO_SHIFT) ? "Shift+" : "",
		    XKeysymToString( keysym ) );
	    return 0;	/* Failure */
	}

	/*
	 * We're going to add to this macro chain, so don't replace this macro.
	 */
	replaceIndex = dummyIndex;

	/*
	 * Make the number of this macro one higher than the last in the macro
	 * chain.
	 */
	modFlags = macro_set_number( modFlags, macroNum+1 );
    }

    else
    {
	modFlags = macro_set_number( modFlags, 0 );

	/* Set replaceIndex to the index of a macro we can replace */
	if( dummyIndex < replaceIndex )
	    replaceIndex = dummyIndex;
    }



    /*
     * Add action to the list of macros (making it bigger if necessary).
     */
    if( replaceIndex == r->nmacros )
    {
	if( r->nmacros == r->maxMacros )
	{
	    /* Get space for more macros*/
	    r->maxMacros += nmacros_increment;
	    r->macros = (macros_t *) rxvt_realloc( r->macros,
					r->maxMacros * sizeof(macros_t));
	}

	r->nmacros++;
    }
    else
    {
	/* Macro action string needs to be freed (as it will be replaced) */
	if( r->macros[replaceIndex].action.str )
	    rxvt_free( r->macros[replaceIndex].action.str );
    }


    /*
     * Set the action string. This malloc's memory so any returns after this
     * should either save action in to a global variable, or free it.
     */
    assert( astring );
    SET_NULL(action.str);   /* Make sure rxvt_set_action won't free non-existent
			       memory */
    if( !rxvt_set_action( &action, astring) )
	return 0; /* Failure: Probably unrecognized action type */

    /*
     * Save macro values in our global macro list.
     */
    r->macros[replaceIndex].keysym	= keysym;
    r->macros[replaceIndex].modFlags	= modFlags;
    r->macros[replaceIndex].action	= action;
    r->macros[replaceIndex].priority	= priority;

    rxvt_dbgmsg(( DBG_DEBUG, DBG_MACROS,
		"Added macro %hu of %hu. Type %s, len %hu, args '%s'.\n",
		replaceIndex, r->maxMacros, macroNames[ action.type ],
		action.len,
		(action.type == MacroFnStr || action.type == MacroFnEsc) ?
		    "(escaped string)" :
		    (IS_NULL(action.str) ?
			"(nil)" : (char*) action.str)));

    return 1;	/* Success */
}