int
rxvt_process_macros( rxvt_t *r, KeySym keysym, XKeyEvent *ev)
{
    macros_t	ck,		/* Storing the keysym and mods of the current
				   key that's pressed. */
		*macro;		/* Macro we find in our saved list corresponding
				   to the current key press */
    int		status;

    if( r->nmacros == 0 )
	return 0;   /* No macro processed */

    /* Copy the modifier mask and keysym into ck */
    ck.modFlags = 0;
    if (ev->state & ShiftMask)		    ck.modFlags |= MACRO_SHIFT;
    if (ev->state & ControlMask)	    ck.modFlags |= MACRO_CTRL;
    if (ev->state & r->h->ModMetaMask)	    ck.modFlags |= MACRO_META;

    /* Use uppercase version so we can ignore caps lock */
    {
        KeySym upper;
        XConvertCase(keysym, &ck.keysym, &upper);
    }

    /* Check if macro ck is in our list of macros. */
    macro = bsearch( &ck, r->macros, r->nmacros, sizeof( macros_t ),
		macro_cmp);
    if (
         /*
          * No macro found.
          */
         IS_NULL(macro)
         || (
              /*
               * Primary only macro in secondary screen.
               */
              (macro->modFlags & MACRO_PRIMARY)
              && AVTS(r)->current_screen != PRIMARY
            )
         || (
              /*
               * When macros are disabled, only the toggle macros macro should
               * work.
               */
              ISSET_OPTION(r, Opt2_disableMacros)
              && macro->action.type != MacroFnToggleMacros
            )
       )
	return 0;   /* No macro processed */

    do
      {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_MACROS, "Processing macro #%d mods %02hhx\n", macro - r->macros, macro->modFlags));
	status = rxvt_dispatch_action( r, &(macro->action), (XEvent*) ev );
      }
    while(
	   status == 1
	   && (++macro - r->macros) < r->nmacros
	   && MACRO_GET_NUMBER( macro->modFlags ) 
	 );

    return status;
}