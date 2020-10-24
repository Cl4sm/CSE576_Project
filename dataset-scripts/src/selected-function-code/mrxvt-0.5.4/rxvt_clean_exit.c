void
rxvt_clean_exit (rxvt_t* r)
{
#ifdef HAVE_X11_SM_SMLIB_H
    if (ISSET_OPTION(r, Opt2_enableSessionMgt))
	rxvt_session_exit (r);
#endif

#if 0
    /*
     * Now kill all child processes, zsh puts them into background if we do not
     * do so.
     *
     * 2006-09-22 gi1242: No! See comments before rxvt_close_all_tabs().
     */
    for (i = 0; i <= LTAB(r); i ++)
	kill (PVTS(r, i)->cmd_pid, SIGHUP);
#endif

    /*
     * 2006-01-27 gi1242: Free all used resources. This used to be done only in
     * debug mode, but I think it's good practice to do it always. It can't hurt
     * anything.
     */
/* #ifdef DEBUG	*/
    rxvt_free_hidden (r);

    /* Destroy windows before other X resources */
    if (IS_WIN(r->TermWin.parent))
    {
	XDestroySubwindows (r->Xdisplay, r->TermWin.parent);
	XDestroyWindow (r->Xdisplay, r->TermWin.parent);
	UNSET_WIN(r->TermWin.parent);
    }

# ifdef HAVE_SCROLLBARS
    rxvt_scrollbar_clean_exit (r);
# endif

# ifdef HAVE_MENUBAR
    rxvt_menubar_clean_exit (r);
# endif

    rxvt_tabbar_clean_exit (r);

    if (NOT_NULL(r->TermWin.font))
	XFreeFont (r->Xdisplay, r->TermWin.font);
# ifndef NO_BOLDFONT
    if (NOT_NULL(r->TermWin.bfont) && r->TermWin.font != r->TermWin.bfont)
    {
	XFreeFont (r->Xdisplay, r->TermWin.bfont);
	SET_NULL(r->TermWin.bfont);
    }
# endif
# ifdef MULTICHAR_SET
    if (NOT_NULL(r->TermWin.mfont) && r->TermWin.font != r->TermWin.mfont)
    {
	XFreeFont (r->Xdisplay, r->TermWin.mfont);
	SET_NULL(r->TermWin.mfont);
    }
# endif
    SET_NULL(r->TermWin.font);	    /* clear font */

# ifdef XFT_SUPPORT
    if (NOT_NULL(r->TermWin.xftfont))
    {
	XftFont *fn;

	XftFontClose (r->Xdisplay, r->TermWin.xftfont);
	xftCloseACS (r->Xdisplay);

	if( (fn = r->TermWin.xftpfont) )
	{
	    SET_NULL(r->TermWin.xftpfont);
	    xftFreeUnusedFont( r, fn);
	}
	if( (fn = r->TermWin.xftPfont) )
	{
	    SET_NULL(r->TermWin.xftPfont);
	    xftFreeUnusedFont( r, fn);
	}

#  ifndef NO_BOLDFONT
	fn = r->TermWin.xftbfont;
	SET_NULL(r->TermWin.xftbfont);
	xftFreeUnusedFont( r, fn);
#  endif

#  ifdef MULTICHAR_SET
	fn = r->TermWin.xftmfont;
	SET_NULL(r->TermWin.xftmfont);
	xftFreeUnusedFont( r, fn);
#  endif
    }
    SET_NULL(r->TermWin.xftfont);   /* clear font */

    /*
     * XXX gi1242 2006-01-27: Xft bug. Patterns passed to XftFontOpenPattern
     * can't always be safely freed.
     */
#  if 0
    if( r->TermWin.xftpattern )
	XftPatternDestroy( r->TermWin.xftpattern);

#  ifdef MULTICHAR_SET
    if( r->TermWin.xftmpattern )
	XftPatternDestroy( r->TermWin.xftmpattern );
#  endif
#  endif
# endif

    if (IS_CURSOR(r->term_pointer))
    {
	XFreeCursor (r->Xdisplay, r->term_pointer);
	UNSET_CURSOR(r->term_pointer);
    }
    if (IS_GC(r->TermWin.gc))
    {
	XFreeGC (r->Xdisplay, r->TermWin.gc);
	UNSET_GC(r->TermWin.gc);
    }
    XCloseDisplay (r->Xdisplay);
    SET_NULL(r->Xdisplay);

#ifdef USE_FIFO
    if( r->fifo_fd != -1 )
    {
	close( r->fifo_fd );
	r->fifo_fd = -1;

	unlink( r->fifo_name );
	rxvt_free( r->fifo_name );  SET_NULL( r->fifo_name );
    }
#endif/*USE_FIFO*/

    rxvt_free (r->tabstop);		    SET_NULL(r->tabstop);
    rxvt_free (r->pixColorsFocus);	    SET_NULL(r->pixColorsFocus);

    if( NOT_NULL( r->pixColorsUnfocus ) )
    {
	rxvt_free (r->pixColorsUnfocus);
	SET_NULL(r->pixColorsUnfocus);
    }

#  ifdef XFT_SUPPORT
    if( NOT_NULL( r->xftColorsUnfocus ) )
    {
	rxvt_free( r->xftColorsUnfocus );
	SET_NULL( r->xftColorsUnfocus );
    }
#  endif /* XFT_SUPPORT */

# ifdef XFT_SUPPORT
    if( NOT_NULL( r->xftColors ) )
    {
	rxvt_free (r->xftColors);
	SET_NULL(r->xftColors);
    }
# endif
    rxvt_free (r->h);		    SET_NULL(r->h);
    rxvt_free (r);		    SET_NULL(r);

/* #endif */	/* DEBUG */

    exit(EXIT_SUCCESS);
}