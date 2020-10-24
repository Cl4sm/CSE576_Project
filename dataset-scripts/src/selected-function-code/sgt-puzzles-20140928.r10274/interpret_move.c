static char *interpret_move(const game_state *state, game_ui *ui,
                            const game_drawstate *ds,
                            int x, int y, int button)
{
    int gx, gy, dx, dy;
    char buf[80];

    button &= ~MOD_MASK;

    gx = X(state, state->gap_pos);
    gy = Y(state, state->gap_pos);

    if (button == CURSOR_RIGHT && gx > 0)
        dx = gx - 1, dy = gy;
    else if (button == CURSOR_LEFT && gx < state->w-1)
        dx = gx + 1, dy = gy;
    else if (button == CURSOR_DOWN && gy > 0)
        dy = gy - 1, dx = gx;
    else if (button == CURSOR_UP && gy < state->h-1)
        dy = gy + 1, dx = gx;
    else if (button == LEFT_BUTTON) {
        dx = FROMCOORD(x);
        dy = FROMCOORD(y);
        if (dx < 0 || dx >= state->w || dy < 0 || dy >= state->h)
            return NULL;               /* out of bounds */
        /*
         * Any click location should be equal to the gap location
         * in _precisely_ one coordinate.
         */
        if ((dx == gx && dy == gy) || (dx != gx && dy != gy))
            return NULL;
    } else
        return NULL;                   /* no move */

    sprintf(buf, "M%d,%d", dx, dy);
    return dupstr(buf);
}