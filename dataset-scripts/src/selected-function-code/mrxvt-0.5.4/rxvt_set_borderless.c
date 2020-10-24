void
rxvt_set_borderless( rxvt_t *r )
{
    Atom	prop;
    CARD32	hints;	    /* KDE/GNOME hints */
    MWMHints	mwmhints;   /* Motif hints */

    hints = (CARD32) 0;
    mwmhints.flags = MWM_HINTS_DECORATIONS;
    mwmhints.decorations = 0;

    /* Motif compatible WM */
    prop = XInternAtom (r->Xdisplay, "_MOTIF_WM_HINTS", True);
    if (IS_ATOM(prop))
	XChangeProperty (r->Xdisplay, r->TermWin.parent, prop, prop,
	    32, PropModeReplace, (unsigned char*) &mwmhints,
	    PROP_MWM_HINTS_ELEMENTS);

    /* GNOME compatible WM */
    prop = XInternAtom (r->Xdisplay, "_WIN_HINTS", True);
    if (IS_ATOM(prop))
	XChangeProperty (r->Xdisplay, r->TermWin.parent, prop, prop,
	    32, PropModeReplace, (unsigned char*) &hints, 1);

    /* KDE compatible WM */
    prop = XInternAtom (r->Xdisplay, "KWM_WIN_DECORATION", True);
    if (IS_ATOM(prop))
	XChangeProperty (r->Xdisplay, r->TermWin.parent, prop, prop,
	    32, PropModeReplace, (unsigned char*) &hints, 1);
}