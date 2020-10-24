static move *solver_reasoning_not_too_big(game_state *state,
                                          int nclues,
                                          const square *clues,
                                          move *buf)
{
    int const w = state->params.w, runmasks[4] = {
        ~(MASK(BLACK) | MASK(EMPTY)),
        MASK(EMPTY),
        ~(MASK(BLACK) | MASK(EMPTY)),
        ~(MASK(BLACK))
    };
    enum {RUN_WHITE, RUN_EMPTY, RUN_BEYOND, RUN_SPACE};

    int i, runlengths[4][4];

    for (i = 0; i < nclues; ++i) {
        int j, k, whites, space;

        const puzzle_size row = clues[i].r, col = clues[i].c;
        int const clue = state->grid[idx(row, col, w)];

        for (j = 0; j < 4; ++j) {
            puzzle_size r = row + dr[j], c = col + dc[j];
            runlengths[RUN_SPACE][j] = 0;
            for (k = 0; k <= RUN_SPACE; ++k) {
                int l = runlength(r, c, dr[j], dc[j], state, runmasks[k]);
                if (k < RUN_SPACE) {
                    runlengths[k][j] = l;
                    r += dr[j] * l;
                    c += dc[j] * l;
                }
                runlengths[RUN_SPACE][j] += l;
            }
        }

        whites = 1;
        for (j = 0; j < 4; ++j) whites += runlengths[RUN_WHITE][j];

        for (j = 0; j < 4; ++j) {
            int const delta = 1 + runlengths[RUN_WHITE][j];
            const puzzle_size r = row + delta * dr[j];
            const puzzle_size c = col + delta * dc[j];

            if (whites == clue) {
                solver_makemove(r, c, M_BLACK, state, &buf);
                continue;
            }

            if (runlengths[RUN_EMPTY][j] == 1 &&
                whites
                + runlengths[RUN_EMPTY][j]
                + runlengths[RUN_BEYOND][j]
                > clue) {
                solver_makemove(r, c, M_BLACK, state, &buf);
                continue;
            }

            if (whites
                + runlengths[RUN_EMPTY][j]
                + runlengths[RUN_BEYOND][j]
                > clue) {
                runlengths[RUN_SPACE][j] =
                    runlengths[RUN_WHITE][j] +
                    runlengths[RUN_EMPTY][j] - 1;

                if (runlengths[RUN_EMPTY][j] == 1)
                    solver_makemove(r, c, M_BLACK, state, &buf);
            }
        }

        space = 1;
        for (j = 0; j < 4; ++j) space += runlengths[RUN_SPACE][j];
        for (j = 0; j < 4; ++j) {
            puzzle_size r = row + dr[j], c = col + dc[j];

            int k = space - runlengths[RUN_SPACE][j];
            if (k >= clue) continue;

            for (; k < clue; ++k, r += dr[j], c += dc[j])
                solver_makemove(r, c, M_WHITE, state, &buf);
        }
    }
    return buf;
}