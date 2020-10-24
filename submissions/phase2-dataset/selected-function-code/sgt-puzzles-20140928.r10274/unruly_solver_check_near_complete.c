static int unruly_solver_check_near_complete(game_state *state,
                                             int *complete, int horizontal,
                                             int *rowcount, int *colcount,
                                             char fill)
{
    int w2 = state->w2, h2 = state->h2;
    int w = w2/2, h = h2/2;

    int dx = horizontal ? 1 : 0, dy = 1 - dx;

    int sx = dx, sy = dy;
    int ex = w2 - dx, ey = h2 - dy;

    int x, y;
    int ret = 0;

    /*
     * This function checks for a row with one Y remaining, then looks
     * for positions that could cause the remaining squares in the row
     * to make 3 X's in a row. Example:
     *
     * Consider the following row:
     * 1 1 0 . . .
     * If the last 1 was placed in the last square, the remaining
     * squares would be 0:
     * 1 1 0 0 0 1
     * This violates the 3 in a row rule. We now know that the last 1
     * shouldn't be in the last cell.
     * 1 1 0 . . 0
     */

    /* Check for any two blank and one filled square */
    for (y = sy; y < ey; y++) {
        /* One type must have 1 remaining, the other at least 2 */
        if (horizontal && (complete[y] < w - 1 || rowcount[y] > w - 2))
            continue;

        for (x = sx; x < ex; x++) {
            int i, i1, i2, i3;
            if (!horizontal
                && (complete[x] < h - 1 || colcount[x] > h - 2))
                continue;

            i = (horizontal ? y : x);
            i1 = (y-dy) * w2 + (x-dx);
            i2 = y * w2 + x;
            i3 = (y+dy) * w2 + (x+dx);

            if (state->grid[i1] == fill && state->grid[i2] == EMPTY
                && state->grid[i3] == EMPTY) {
                /*
                 * Temporarily fill the empty spaces with something else.
                 * This avoids raising the counts for the row and column
                 */
                state->grid[i2] = BOGUS;
                state->grid[i3] = BOGUS;

#ifdef STANDALONE_SOLVER
                if (solver_verbose) {
                    printf("Solver: Row %i nearly satisfied for %c\n", i,
                           (fill != N_ZERO ? '0' : '1'));
                }
#endif
                ret +=
                    unruly_solver_fill_row(state, i, horizontal, rowcount,
                                         colcount, fill);

                state->grid[i2] = EMPTY;
                state->grid[i3] = EMPTY;
            }

            else if (state->grid[i1] == EMPTY && state->grid[i2] == fill
                     && state->grid[i3] == EMPTY) {
                state->grid[i1] = BOGUS;
                state->grid[i3] = BOGUS;

#ifdef STANDALONE_SOLVER
                if (solver_verbose) {
                    printf("Solver: Row %i nearly satisfied for %c\n", i,
                           (fill != N_ZERO ? '0' : '1'));
                }
#endif
                ret +=
                    unruly_solver_fill_row(state, i, horizontal, rowcount,
                                         colcount, fill);

                state->grid[i1] = EMPTY;
                state->grid[i3] = EMPTY;
            }

            else if (state->grid[i1] == EMPTY && state->grid[i2] == EMPTY
                     && state->grid[i3] == fill) {
                state->grid[i1] = BOGUS;
                state->grid[i2] = BOGUS;

#ifdef STANDALONE_SOLVER
                if (solver_verbose) {
                    printf("Solver: Row %i nearly satisfied for %c\n", i,
                           (fill != N_ZERO ? '0' : '1'));
                }
#endif
                ret +=
                    unruly_solver_fill_row(state, i, horizontal, rowcount,
                                         colcount, fill);

                state->grid[i1] = EMPTY;
                state->grid[i2] = EMPTY;
            }

            else if (state->grid[i1] == EMPTY && state->grid[i2] == EMPTY
                     && state->grid[i3] == EMPTY) {
                state->grid[i1] = BOGUS;
                state->grid[i2] = BOGUS;
                state->grid[i3] = BOGUS;

#ifdef STANDALONE_SOLVER
                if (solver_verbose) {
                    printf("Solver: Row %i nearly satisfied for %c\n", i,
                           (fill != N_ZERO ? '0' : '1'));
                }
#endif
                ret +=
                    unruly_solver_fill_row(state, i, horizontal, rowcount,
                                         colcount, fill);

                state->grid[i1] = EMPTY;
                state->grid[i2] = EMPTY;
                state->grid[i3] = EMPTY;
            }
        }
    }

    return ret;
}