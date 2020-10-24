void
rxvt_cleanup_macros( rxvt_t *r )
{
    unsigned i, nDummyMacros = 0;

    if( r->nmacros == 0 ) return; /* Nothing to be done */

    for( i = 0; i < r->nmacros; i++)
    {
	if(
	     r->macros[i].action.type == MacroFnDummy ||
	     NOT_KEYSYM(r->macros[i].keysym)
	  )
	{
	    /*
	     * Dummy macro needs to be deleted. Make sure this macro comes first
	     * in the macro list.
	     *
	     * 2006-03-06 gi1242: Would be more efficient if we made sure that
	     * this macro was last in the list, however that would involve
	     * knowing what the max keysym value is. Could be different on
	     * different architectures.
	     */
	    r->macros[i].keysym	    = 0;
	    r->macros[i].modFlags   = 0;

	    if (NOT_NULL(r->macros[i].action.str))
	    {
	        rxvt_free( r->macros[i].action.str );
	        SET_NULL(r->macros[i].action.str); /* Probably unnecessary */
	    }

	    nDummyMacros++;
	}
    } /* for */

    /*
     * The macro list now needs to be sorted on keysym. When we look for macros,
     * we assume the macro list is sorted, so we can use a binary search to
     * lookup macros quickly.
     */
    qsort( r->macros, r->nmacros, sizeof( macros_t ), macro_cmp);

    /* Remove dummy macros from our list */
    MEMMOVE( r->macros, r->macros + nDummyMacros,
	    (r->nmacros - nDummyMacros) * sizeof( macros_t ) );
    r->nmacros -= nDummyMacros;

    /* Shrink our macros list */
    if( r->nmacros < r->maxMacros )
    {
	r->macros = rxvt_realloc( r->macros, r->nmacros * sizeof( macros_t ));
	r->maxMacros = r->nmacros;
    }

    rxvt_dbgmsg(( DBG_DEBUG, DBG_MACROS,
		"Read %d macros. (Have space for %d macros)\n",
		r->nmacros, r->maxMacros));
}