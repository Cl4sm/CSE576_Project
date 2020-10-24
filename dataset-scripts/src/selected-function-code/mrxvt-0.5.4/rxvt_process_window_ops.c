void
rxvt_process_window_ops(rxvt_t* r, int page, const int *args, unsigned int nargs)
{
    int			x, y;
    unsigned int	w, h;
    XWindowAttributes	wattr;
    Window		wdummy;


    if (nargs == 0)
	return;
    switch (args[0])
    {
	/* commands */
	case 1:		/* deiconify window */
	    XMapWindow (r->Xdisplay, r->TermWin.parent);
	    break;
	case 2:		/* iconify window */
	    XIconifyWindow (r->Xdisplay, r->TermWin.parent,
		DefaultScreen(r->Xdisplay));
	    break;
	case 3:		/* set position (pixels) */
	    XMoveWindow (r->Xdisplay, r->TermWin.parent, args[1], args[2]);
	    break;
	case 4:		/* set size (pixels) */
	    w = (unsigned int) args[2];
	    h = (unsigned int) args[1];
	    XResizeWindow (r->Xdisplay, r->TermWin.parent, w, h);
	    r->h->want_resize = 1;
	    break;
	case 5:		/* raise window */
	    XRaiseWindow (r->Xdisplay, r->TermWin.parent);
	    break;
	case 6:		/* lower window */
	    XLowerWindow (r->Xdisplay, r->TermWin.parent);
	    break;
	case 7:		/* refresh window */
	    rxvt_scr_touch (r, page, True);
	    break;
	case 8:		/* set size (chars) */
	    w = (unsigned int) (Width2Pixel(args[2]) + r->szHint.base_width);
	    h = (unsigned int) (Height2Pixel(args[1]) + r->szHint.base_height);
	    XResizeWindow (r->Xdisplay, r->TermWin.parent, w, h);
	    r->h->want_resize = 1;
	    break;
	default:
	    if (args[0] >= 24)
	    {
		/* set height (chars) */
		w = (unsigned int) r->szHint.width;
		h = (unsigned int) (args[1] * r->TermWin.fheight + r->szHint.base_height);
		XResizeWindow (r->Xdisplay, r->TermWin.parent, w, h);
		r->h->want_resize = 1;
	    }
	    break;

	/* reports - some output format copied from XTerm */
	case 11:	    /* report window state */
	    XGetWindowAttributes(r->Xdisplay, r->TermWin.parent, &wattr);
	    rxvt_tt_printf(r, page, "\033[%dt",
		wattr.map_state == IsViewable ? 1 : 2);
	    break;
	case 13:	    /* report window position */
	    XGetWindowAttributes(r->Xdisplay, r->TermWin.parent, &wattr);
	    XTranslateCoordinates(r->Xdisplay, r->TermWin.parent,
		wattr.root, -wattr.border_width, -wattr.border_width,
		&x, &y, &wdummy);
	    rxvt_tt_printf(r, page, "\033[3;%d;%dt", x, y);
	    break;
	case 14:	    /* report window size (pixels) */
	    XGetWindowAttributes(r->Xdisplay, r->TermWin.parent, &wattr);
	    rxvt_tt_printf(r, page, "\033[4;%d;%dt", wattr.height,
		wattr.width);
	    break;
	case 18:	    /* report window size (chars) */
	    rxvt_tt_printf(r, page, "\033[8;%d;%dt", r->TermWin.nrow,
		r->TermWin.ncol);
	    break;
    }
}