static int unruly_solver_check_complete_nums(game_state *state,
                                             int *complete, int horizontal,
                                             int *rowcount, int *colcount,
                                             char fill)
{
    int w2 = state->w2, h2 = state->h2;
    int count = (horizontal ? h2 : w2); /* number of rows to check */
    int target = (horizontal ? w2 : h2) / 2; /* target number of 0s/1s */
    int *other = (horizontal ? rowcount : colcount);

    int ret = 0;

    int i;
    /* Check for completed rows/cols for one number, then fill in the rest */
    for (i = 0; i < count; i++) {
        if (complete[i] == target && other[i] < target) {
#ifdef STANDALONE_SOLVER
            if (solver_verbose) {
                printf("Solver: Row %i satisfied for %c\n", i,
                       (fill != N_ZERO ? '0' : '1'));
            }
#endif
            ret += unruly_solver_fill_row(state, i, horizontal, rowcount,
                                          colcount, fill);
        }
    }

    return ret;
}