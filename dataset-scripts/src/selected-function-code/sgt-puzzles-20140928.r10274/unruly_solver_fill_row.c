static int unruly_solver_fill_row(game_state *state, int i, int horizontal,
                                  int *rowcount, int *colcount, char fill)
{
    int ret = 0;
    int w2 = state->w2, h2 = state->h2;
    int j;

#ifdef STANDALONE_SOLVER
    if (solver_verbose) {
        printf("Solver: Filling %s %i with %c:",
               (horizontal ? "Row" : "Col"), i,
               (fill == N_ZERO ? '0' : '1'));
    }
#endif
    /* Place a number in every empty square in a row/column */
    for (j = 0; j < (horizontal ? w2 : h2); j++) {
        int p = (horizontal ? i * w2 + j : j * w2 + i);

        if (state->grid[p] == EMPTY) {
#ifdef STANDALONE_SOLVER
            if (solver_verbose) {
                printf(" (%i,%i)", (horizontal ? j : i),
                       (horizontal ? i : j));
            }
#endif
            ret++;
            state->grid[p] = fill;
            rowcount[(horizontal ? i : j)]++;
            colcount[(horizontal ? j : i)]++;
        }
    }

#ifdef STANDALONE_SOLVER
    if (solver_verbose) {
        printf("\n");
    }
#endif

    return ret;
}