static void debug_state(game_state *state)
{
    int x, y;
    char c = '?';

    for (y = 0; y < state->h; y++) {
        for (x = 0; x < state->w; x++) {
            c = '.';
            if (GRID(state, flags, x, y) & F_BLACK) {
                if (GRID(state, flags, x, y) & F_NUMBERED)
                    c = GRID(state, lights, x, y) + '0';
                else
                    c = '#';
            } else {
                if (GRID(state, flags, x, y) & F_LIGHT)
                    c = 'O';
                else if (GRID(state, flags, x, y) & F_IMPOSSIBLE)
                    c = 'X';
            }
            debug(("%c", (int)c));
        }
        debug(("     "));
        for (x = 0; x < state->w; x++) {
            if (GRID(state, flags, x, y) & F_BLACK)
                c = '#';
            else {
                c = (GRID(state, flags, x, y) & F_LIGHT) ? 'A' : 'a';
                c += GRID(state, lights, x, y);
            }
            debug(("%c", (int)c));
        }
        debug(("\n"));
    }
}