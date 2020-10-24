static int unruly_solve_game(game_state *state,
                             struct unruly_scratch *scratch, int diff)
{
    int done, maxdiff = -1;

    while (TRUE) {
        done = 0;

        /* Check for impending 3's */
        done += unruly_solver_check_all_threes(state, scratch);

        /* Keep using the simpler techniques while they produce results */
        if (done) {
            if (maxdiff < DIFF_EASY)
                maxdiff = DIFF_EASY;
            continue;
        }

        /* Check for completed rows */
        done += unruly_solver_check_all_complete_nums(state, scratch);

        if (done) {
            if (maxdiff < DIFF_EASY)
                maxdiff = DIFF_EASY;
            continue;
        }

        /* Check for impending failures of row/column uniqueness, if
         * it's enabled in this game mode */
        if (state->unique) {
            done += unruly_solver_check_all_uniques(state, scratch);

            if (done) {
                if (maxdiff < DIFF_EASY)
                    maxdiff = DIFF_EASY;
                continue;
            }
        }

        /* Normal techniques */
        if (diff < DIFF_NORMAL)
            break;

        /* Check for nearly completed rows */
        done += unruly_solver_check_all_near_complete(state, scratch);

        if (done) {
            if (maxdiff < DIFF_NORMAL)
                maxdiff = DIFF_NORMAL;
            continue;
        }

        break;
    }
    return maxdiff;
}