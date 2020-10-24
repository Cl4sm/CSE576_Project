{
    /* Set up tty flags in raw and cooked structures.
     * Do this before any termcap-initialisation stuff
     * so that start sequences can be sent.*/

    getstate(&cooked_state);
    raw_state = cooked_state;

    cfmakeraw(&raw_state);

    /* Now set up the terminal interface. */
    ttyopen();
    clrscr();

    /* Go into raw/cbreak mode, and say so. */
    w_setstate(&raw_state);

    termmode = m_QDE;
}
