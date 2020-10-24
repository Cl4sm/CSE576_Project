static int unruly_solver_check_all_threes(game_state *state,
                                          struct unruly_scratch *scratch)
{
    int ret = 0;

    ret +=
        unruly_solver_check_threes(state, scratch->zeros_rows,
                                   scratch->zeros_cols, TRUE, N_ONE, N_ZERO);
    ret +=
        unruly_solver_check_threes(state, scratch->ones_rows,
                                   scratch->ones_cols, TRUE, N_ZERO, N_ONE);
    ret +=
        unruly_solver_check_threes(state, scratch->zeros_rows,
                                   scratch->zeros_cols, FALSE, N_ONE,
                                   N_ZERO);
    ret +=
        unruly_solver_check_threes(state, scratch->ones_rows,
                                   scratch->ones_cols, FALSE, N_ZERO, N_ONE);

    return ret;
}