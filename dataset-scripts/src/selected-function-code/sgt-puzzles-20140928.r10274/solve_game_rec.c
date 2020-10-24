static solver_state *solve_game_rec(const solver_state *sstate_start)
{
    solver_state *sstate;

    /* Index of the solver we should call next. */
    int i = 0;
    
    /* As a speed-optimisation, we avoid re-running solvers that we know
     * won't make any progress.  This happens when a high-difficulty
     * solver makes a deduction that can only help other high-difficulty
     * solvers.
     * For example: if a new 'dline' flag is set by dline_deductions, the
     * trivial_deductions solver cannot do anything with this information.
     * If we've already run the trivial_deductions solver (because it's
     * earlier in the list), there's no point running it again.
     *
     * Therefore: if a solver is earlier in the list than "threshold_index",
     * we don't bother running it if it's difficulty level is less than
     * "threshold_diff".
     */
    int threshold_diff = 0;
    int threshold_index = 0;
    
    sstate = dup_solver_state(sstate_start);

    check_caches(sstate);

    while (i < NUM_SOLVERS) {
        if (sstate->solver_status == SOLVER_MISTAKE)
            return sstate;
        if (sstate->solver_status == SOLVER_SOLVED ||
            sstate->solver_status == SOLVER_AMBIGUOUS) {
            /* solver finished */
            break;
        }

        if ((solver_diffs[i] >= threshold_diff || i >= threshold_index)
            && solver_diffs[i] <= sstate->diff) {
            /* current_solver is eligible, so use it */
            int next_diff = solver_fns[i](sstate);
            if (next_diff != DIFF_MAX) {
                /* solver made progress, so use new thresholds and
                * start again at top of list. */
                threshold_diff = next_diff;
                threshold_index = i;
                i = 0;
                continue;
            }
        }
        /* current_solver is ineligible, or failed to make progress, so
         * go to the next solver in the list */
        i++;
    }

    if (sstate->solver_status == SOLVER_SOLVED ||
        sstate->solver_status == SOLVER_AMBIGUOUS) {
        /* s/LINE_UNKNOWN/LINE_NO/g */
        array_setall(sstate->state->lines, LINE_UNKNOWN, LINE_NO,
                     sstate->state->game_grid->num_edges);
        return sstate;
    }

    return sstate;
}