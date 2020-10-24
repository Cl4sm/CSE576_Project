void
mouse_report(XButtonEvent * ev)
{
    int             button_number, key_state = 0;
    int             x, y;

    x = ev->x;
    y = ev->y;
    pixel_position(&x, &y);

    button_number = ((MEvent.button == AnyButton) ? 3 :
		     (MEvent.button - Button1));

    if (PrivateModes & PrivMode_MouseX10) {
    /*
     * do not report ButtonRelease
     * no state info allowed
     */
	key_state = 0;
	if (button_number == 3)
	    return;
    } else {
    /* let's be explicit here ... from <X11/X.h>
     * #define ShiftMask        (1<<0)
     * #define ControlMask        (1<<2)
     * #define Mod1Mask           (1<<3)
     *
     * and XTerm mouse reporting needs these values:
     *   4 = Shift
     *   8 = Meta
     *  16 = Control
     * plus will add in our own Double-Click reporting
     *  32 = Double Click
     */
	key_state = (((MEvent.state & (ShiftMask | ControlMask))
		      + ((MEvent.state & Mod1Mask) ? 2 : 0)
#ifdef MOUSE_REPORT_DOUBLECLICK
		      + (MEvent.clicks > 1 ? 8 : 0)
#endif
		     ) << 2);
	/* Report mouse wheel events. */
	if (ev->button == Button4 || ev->button == Button5) {
	    key_state |= 1 << 6;
	    button_number = ev->button - Button4;
	}
    }

#ifdef DEBUG_MOUSEREPORT
    fprintf(stderr, "Mouse [");
    if (key_state & 16)
	fputc('C', stderr);
    if (key_state & 4)
	fputc('S', stderr);
    if (key_state & 2)
	fputc('A', stderr);
    if (key_state & 32)
	fputc('2', stderr);
    fprintf(stderr, "]: <%d>, %d/%d\n",
	    button_number,
	    x + 1,
	    y + 1);
#else
    tt_printf((unsigned char *) "\033[M%c%c%c",
	      (32 + button_number + key_state),
	      (32 + x + 1),
	      (32 + y + 1));
#endif
}
