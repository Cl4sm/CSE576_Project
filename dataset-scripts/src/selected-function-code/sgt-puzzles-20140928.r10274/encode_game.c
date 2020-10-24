static char *encode_game(game_state *state)
{
    char *ret, *p;
    int wh = state->w*state->h, run, x, y;
    struct island *is;

    ret = snewn(wh + 1, char);
    p = ret;
    run = 0;
    for (y = 0; y < state->h; y++) {
        for (x = 0; x < state->w; x++) {
            is = INDEX(state, gridi, x, y);
            if (is) {
                if (run) {
                    *p++ = ('a'-1) + run;
                    run = 0;
                }
                if (is->count < 10)
                    *p++ = '0' + is->count;
                else
                    *p++ = 'A' + (is->count - 10);
            } else {
                if (run == 26) {
                    *p++ = ('a'-1) + run;
                    run = 0;
                }
                run++;
            }
        }
    }
    if (run) {
        *p++ = ('a'-1) + run;
        run = 0;
    }
    *p = '\0';
    assert(p - ret <= wh);

    return ret;
}