void
scr_poweron(void)
{
    D_SCREEN((stderr, "scr_poweron()"));

    MEMSET(charsets, 'B', sizeof(charsets));
    rvideo = 0;
    swap.tscroll = 0;
    swap.bscroll = TermWin.nrow - 1;
    screen.cur.row = screen.cur.col = swap.cur.row = swap.cur.col = 0;
    screen.charset = swap.charset = 0;
    screen.flags = swap.flags = Screen_DefaultFlags;

    scr_cursor(SAVE);

    scr_release();
    prev_nrow = -1;
    prev_bcol = -1;
    scr_reset();

    scr_clear();
    scr_refresh(SLOW_REFRESH);
    Gr_reset();
}
