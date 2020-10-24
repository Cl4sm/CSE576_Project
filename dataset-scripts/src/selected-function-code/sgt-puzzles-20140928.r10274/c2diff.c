int c2diff(int w, int h, int cx, int cy, int button)
{
    int diff = 0;

    assert(IS_CURSOR_MOVE(button));

    /* Obvious moves around edge. */
    if (cy == -1)
        diff = (button == CURSOR_RIGHT) ? +1 : (button == CURSOR_LEFT) ? -1 : diff;
    if (cy == h)
        diff = (button == CURSOR_RIGHT) ? -1 : (button == CURSOR_LEFT) ? +1 : diff;
    if (cx == -1)
        diff = (button == CURSOR_UP) ? +1 : (button == CURSOR_DOWN) ? -1 : diff;
    if (cx == w)
        diff = (button == CURSOR_UP) ? -1 : (button == CURSOR_DOWN) ? +1 : diff;

    if (button == CURSOR_LEFT && cx == w && (cy == 0 || cy == h-1))
        diff = (cy == 0) ? -1 : +1;
    if (button == CURSOR_RIGHT && cx == -1 && (cy == 0 || cy == h-1))
        diff = (cy == 0) ? +1 : -1;
    if (button == CURSOR_DOWN && cy == -1 && (cx == 0 || cx == w-1))
        diff = (cx == 0) ? -1 : +1;
    if (button == CURSOR_UP && cy == h && (cx == 0 || cx == w-1))
        diff = (cx == 0) ? +1 : -1;

    debug(("cx,cy = %d,%d; w%d h%d, diff = %d", cx, cy, w, h, diff));
    return diff;
}