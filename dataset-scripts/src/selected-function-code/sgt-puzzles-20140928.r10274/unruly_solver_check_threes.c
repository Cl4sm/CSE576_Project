static int unruly_solver_check_threes(game_state *state, int *rowcount,
                                      int *colcount, int horizontal,
                                      char check, char block)
{
    int w2 = state->w2, h2 = state->h2;

    int dx = horizontal ? 1 : 0, dy = 1 - dx;
    int sx = dx, sy = dy;
    int ex = w2 - dx, ey = h2 - dy;

    int x, y;
    int ret = 0;

    /* Check for any three squares which almost form three in a row */
    for (y = sy; y < ey; y++) {
        for (x = sx; x < ex; x++) {
            int i1 = (y-dy) * w2 + (x-dx);
            int i2 = y * w2 + x;
            int i3 = (y+dy) * w2 + (x+dx);

            if (state->grid[i1] == check && state->grid[i2] == check
                && state->grid[i3] == EMPTY) {
                ret++;
#ifdef STANDALONE_SOLVER
                if (solver_verbose) {
                    printf("Solver: %i,%i and %i,%i confirm %c at %i,%i\n",
                           i1 % w2, i1 / w2, i2 % w2, i2 / w2,
                           (block == N_ONE ? '1' : '0'), i3 % w2,
                           i3 / w2);
                }
#endif
                state->grid[i3] = block;
                rowcount[i3 / w2]++;
                colcount[i3 % w2]++;
            }
            if (state->grid[i1] == check && state->grid[i2] == EMPTY
                && state->grid[i3] == check) {
                ret++;
#ifdef STANDALONE_SOLVER
                if (solver_verbose) {
                    printf("Solver: %i,%i and %i,%i confirm %c at %i,%i\n",
                           i1 % w2, i1 / w2, i3 % w2, i3 / w2,
                           (block == N_ONE ? '1' : '0'), i2 % w2,
                           i2 / w2);
                }
#endif
                state->grid[i2] = block;
                rowcount[i2 / w2]++;
                colcount[i2 % w2]++;
            }
            if (state->grid[i1] == EMPTY && state->grid[i2] == check
                && state->grid[i3] == check) {
                ret++;
#ifdef STANDALONE_SOLVER
                if (solver_verbose) {
                    printf("Solver: %i,%i and %i,%i confirm %c at %i,%i\n",
                           i2 % w2, i2 / w2, i3 % w2, i3 / w2,
                           (block == N_ONE ? '1' : '0'), i1 % w2,
                           i1 / w2);
                }
#endif
                state->grid[i1] = block;
                rowcount[i1 / w2]++;
                colcount[i1 % w2]++;
            }
        }
    }

    return ret;
}