static move *do_solve(game_state *state,
                      int nclues,
                      const square *clues,
                      move *move_buffer,
                      int difficulty)
{
    struct move *buf = move_buffer, *oldbuf;
    int i;

    do {
        oldbuf = buf;
        for (i = 0; i < lenof(reasonings) && i <= difficulty; ++i) {
            /* only recurse if all else fails */
            if (i == DIFF_RECURSION && buf > oldbuf) continue;
            buf = (*reasonings[i])(state, nclues, clues, buf);
            if (buf == NULL) return NULL;
        }
    } while (buf > oldbuf);

    return buf;
}