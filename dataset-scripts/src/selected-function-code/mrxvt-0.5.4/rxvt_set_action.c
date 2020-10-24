Bool
rxvt_set_action	    (action_t *action, char *astring)
{
    unsigned short type, len;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_MACROS, "Setting action '%s'\n", astring));
    /*
     * Match head of "astring" to a name in macroNames to figure out the macro
     * type.
     */
    for( type = 0; type < NMACRO_FUNCS; type++)
    {
	if( (len = rxvt_str_match( astring, macroNames[type])) )
	{
	    /* Matched a macroName at the start of astring */
	    if( astring[len] && !isspace( astring[len] ) )
		/* Not delimited by a space */
		continue;

	    /* Skip macroName and delimiting spaces */
	    astring += len;
	    while( *astring && isspace( *astring ) ) astring++;

	    /* Exit for loop early */
	    break;
	}
    }

    if( type == NMACRO_FUNCS )
    {
	rxvt_msg (DBG_ERROR, DBG_MACROS,  "Action %s is not of known type", astring);
	return False; /* Failure: No matching macro name */
    }

    /*
     * Setup values in action
     */
    action->type	= type;

    /*
     * Interpolate escape sequences into action. XXX: Should we only do this for
     * MacroFnStr and MacroFnEsc?.
     */
    len	= rxvt_str_escaped( astring );

    /* All macros exept MacroFnStr and MacroFnEsc have null terminated string */
    if( type != MacroFnStr && type != MacroFnEsc && len > 0 && astring[len-1] )
	astring[ len++ ] = 0;	/* Since astring was null terminated,
				   astring[len] is certainly part of the memory
				   in astring. */

    action->len	    = len;

    /* Set action->str. If any data is previously there, realloc it. */
    if( len > 0 )
    {
	action->str = (unsigned char *) rxvt_realloc( action->str,
						len * sizeof(unsigned char));
	MEMCPY( action->str, astring, len);
    }
    else
    if (NOT_NULL(action->str))
    {
	rxvt_free( action->str );
	SET_NULL(action->str);
    }
    return True;
}