int
rxvt_init_vars(rxvt_t *r)
{
    register int	i;
    struct rxvt_hidden*	h;


    MEMSET(r, 0, sizeof(rxvt_t));

    h = r->h = (struct rxvt_hidden *)rxvt_calloc(1, sizeof(struct rxvt_hidden));

    for (i = 0; i < MAX_PAGES; i ++)
    {
	/* Initialize vts_idx for each term_t structure */
	r->vterm[i].vts_idx = -1;
	/* Initialize each vts pointer */
	SET_NULL(r->vts[i]);
    }

    SET_NULL(r->Xdisplay);
#ifdef USE_XIM
    SET_NULL(r->TermWin.fontset);
#endif
    SET_NULL(r->TermWin.font);
#ifdef MULTICHAR_SET
    SET_NULL(r->TermWin.mfont);
#endif
#ifndef NO_BOLDFONT
    SET_NULL(r->TermWin.bfont);
#endif

#ifdef XFT_SUPPORT
    SET_NULL(r->TermWin.xftpattern);
    SET_NULL(r->TermWin.xftfont);
    SET_NULL(r->TermWin.xftpfont);
    SET_NULL(r->TermWin.xftPfont);
# ifndef NO_BOLDFONT
    SET_NULL(r->TermWin.xftbfont);
# endif	/* NO_BOLDFONT */
# ifdef MULTICHAR_SET
#  ifdef HAVE_ICONV_H
    r->TermWin.xfticonv = (iconv_t) -1;
#  endif
    SET_NULL(r->TermWin.xftmpattern);
    SET_NULL(r->TermWin.xftmfont);
# endif	/* MULTICHAR_SET */
#endif	/* XFT_SUPPORT */

    UNSET_ATOM(h->xa[XA_COMPOUND_TEXT]);
    UNSET_ATOM(h->xa[XA_MULTIPLE]);
    UNSET_ATOM(h->xa[XA_TARGETS]);
    UNSET_ATOM(h->xa[XA_TEXT]);
    UNSET_ATOM(h->xa[XA_TIMESTAMP]);
    UNSET_ATOM(h->xa[XA_VT_SELECTION]);
    UNSET_ATOM(h->xa[XA_INCR]);
    h->locale = NULL;

# ifdef HAVE_MENUBAR
    SET_NULL(h->BuildMenu);
    SET_NULL(h->ActiveMenu);
    SET_NULL(h->popupMenu[0]);
    SET_NULL(h->popupMenu[1]);
    SET_NULL(h->popupMenu[2]);
    h->showingMenu = 0;

    /* Set the current menubar to empty defaults */
    SET_NULL(h->MenuBar.head);
    SET_NULL(h->MenuBar.tail);
    SET_NULL(h->MenuBar.title);
# endif

# ifdef USE_XIM
    SET_NULL(h->Input_Context);
# endif
    /* SET_NULL(h->v_bufstr); */
    SET_NULL(h->buffer);

# ifdef TRANSPARENT
    h->am_pixmap_trans = 0;
    h->am_transparent  = 0;
    UNSET_PIXMAP(h->rootPixmap);
    h->bgRefreshInterval = DEFAULT_BG_REFRESH_INTERVAL;
    h->lastCNotify.tv_sec = 0;	    /* No BG update pending */
# endif

    /* Initialize timeouts to 0 */
    for( i=NUM_TIMEOUTS; i--;)
        h->timeout[i].tv_sec = 0;


    /* Back to undocumented code :) */
    h->MEvent.time = CurrentTime;
    h->MEvent.button = AnyButton;
    r->Options[0] = DEFAULT_OPTIONS;
    r->Options[1] = DEFAULT_OPTIONS2;
    r->Options[2] = DEFAULT_OPTIONS3;
    r->Options[3] = DEFAULT_OPTIONS4;
    h->want_clip_refresh = 0;
    /*
     * We only want to set want_resize when we call XResizeWindow. In that
     * case if XResizeWindow fails, we know that we called it, and can run
     * our internal resize routines anyway (e.g. put the tabbar in place)
     */
    h->want_resize = 0;
    h->ttygid = -1;
    r->Xfd = -1;
#ifdef USE_FIFO
    r->fifo_fd = -1;
#endif
    r->ndead_childs = 0;

    r->nAsyncChilds = 0;

    /* default values */
#ifdef NO_FRILLS
    r->TermWin.int_bwidth = DEFAULT_INTERNALBORDERWIDTH;
    r->TermWin.ext_bwidth = DEFAULT_EXTERNALBORDERWIDTH;
#else
    r->TermWin.int_bwidth = (INTERNALBORDERWIDTH >= 0 && INTERNALBORDERWIDTH <= MAX_INTERNALBORDERWIDTH) ? INTERNALBORDERWIDTH : DEFAULT_INTERNALBORDERWIDTH;
    r->TermWin.ext_bwidth = (EXTERNALBORDERWIDTH >= 0 && EXTERNALBORDERWIDTH <= MAX_EXTERNALBORDERWIDTH) ? EXTERNALBORDERWIDTH : DEFAULT_EXTERNALBORDERWIDTH;
#endif

#ifndef NO_LINESPACE
    r->TermWin.lineSpace = (LINESPACE >= 0 && LINESPACE <= MAX_LINESPACE) ? LINESPACE : DEFAULT_LINESPACE;
#endif

#ifdef CURSOR_BLINK
    r->h->blinkInterval = DEFAULT_BLINK_TIME;
#endif
#ifdef POINTER_BLANK
    r->h->pointerBlankDelay = DEFAULT_BLANKDELAY;
#endif

    /* Initialize selection data */
#ifndef NO_NEW_SELECTION
    r->selection_style = NEW_SELECT;
#else
    r->selection_style = OLD_SELECT;
#endif
    r->selection.vt = -1;
    r->selection.op = SELECTION_CLEAR;
    r->selection.screen = PRIMARY;
    r->selection.clicks = 0;
    SET_NULL(r->selection.text);
    r->selection.len = 0;
    r->selection.beg.row = 0;
    r->selection.beg.col = 0;
    r->selection.end.row = 0;
    r->selection.end.col = 0;

#ifndef NO_BRIGHTCOLOR
    h->colorfgbg = DEFAULT_RSTYLE;
#endif
#ifdef GREEK_SUPPORT
    h->ks_greekmodeswith = GREEK_KEYBOARD_MODESWITCH;
#endif
    h->refresh_type = SLOW_REFRESH;
    UNSET_REGION(h->refreshRegion);	    /* Will be created when needed */
    h->prev_nrow = h->prev_ncol = 0;

    r->encoding_method = ENC_NOENC;
    h->multichar_decode = rxvt_decode_dummy;

    h->oldcursor.row = h->oldcursor.col = -1;
    h->last_bot = h->last_state = -1;

#ifdef HAVE_X11_SM_SMLIB_H
    SET_NULL(r->TermWin.sm_conn);
    SET_NULL(r->TermWin.ice_conn);
    r->TermWin.ice_fd = -1;
    SET_NULL(r->TermWin.sm_client_id);
#endif

#ifdef USE_FIFO
    {
	char fifo_name[FILENAME_MAX];

	sprintf( fifo_name, "/tmp/.mrxvt-%d", getpid() );
	r->fbuf_ptr = r->fifo_buf;
	r->fifo_name = STRDUP( fifo_name );
    }
#endif/*USE_FIFO*/

    r->tabClicked = -1; /* No tab has been clicked by user */

    h->allowedxerror = 0;
    h->xerror_return = Success;
    return 0;
}