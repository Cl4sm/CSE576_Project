void ttyopen()
{
    char    tcbuf[4096];        /* buffer for termcap entry */
    char    *termtype;          /* terminal type */
    static  char strings[512];  /* space for storing strings */
    char    *strp = strings;    /* ptr to space left in strings */
    int iv;

    termtype = getenv("TERM");
    if (termtype == NULL) fail("Can't find TERM in environment.");

    switch (tgetent(tcbuf, termtype)) {
    case -1: fail("Can't open termcap.");    /*NOTREACHED*/
    case 0:  fail("Can't find entry for your terminal in termcap.");
    }

    /* Screen dimensions. */
    iv = tgetnum("co");
    /* check that x is there */
    if (iv <= 0) fail("`co' entry in termcap is invalid or missing.");

    COLS = (unsigned) iv;

    iv = tgetnum("li");
    /* check that y is there */
    if (iv <= 0) fail("`li' entry in termcap is invalid or missing.");

    LINES = (unsigned) iv;

    /* set output screen height and width */
    screen_height=LINES;
    screen_width=COLS;

    /* output control */
    CL = tgetstr("cl", &strp);  /* clear erase */
    CM = tgetstr("cm", &strp);  /* move goto */
    SO = tgetstr("so", &strp);  /* standout mode start */
    SE = tgetstr("se", &strp);  /* standout mode end */

    if (CM == NULL) {
        fail("qde can't work without cursor motion.");
    }

}
