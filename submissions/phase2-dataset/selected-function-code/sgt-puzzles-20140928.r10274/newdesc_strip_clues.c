static int newdesc_strip_clues(game_state *state, int *shuffle_1toN)
{
    int const w = state->params.w, n = w * state->params.h;

    move *const move_buffer = snewn(n, move);
    move *buf;
    game_state *dupstate;

    /*
     * do a partition/pivot of shuffle_1toN into three groups:
     * (1) squares rotationally-symmetric to (3)
     * (2) squares not in (1) or (3)
     * (3) black squares
     *
     * They go from [0, left), [left, right) and [right, n) in
     * shuffle_1toN (and from there into state->grid[ ])
     *
     * Then, remove clues from the grid one by one in shuffle_1toN
     * order, until the solver becomes unhappy.  If we didn't remove
     * all of (1), return (-1).  Else, we're happy.
     */

    /* do the partition */
    int clues_removed, k = 0, left = 0, right = n;

    for (;; ++k) {
        while (k < right && state->grid[shuffle_1toN[k]] == BLACK) {
            --right;
            SWAP(int, shuffle_1toN[right], shuffle_1toN[k]);
            assert(state->grid[shuffle_1toN[right]] == BLACK);
        }
        if (k >= right) break;
        assert (k >= left);
        if (state->grid[rotate(shuffle_1toN[k])] == BLACK) {
            SWAP(int, shuffle_1toN[k], shuffle_1toN[left]);
            ++left;
        }
        assert (state->grid[rotate(shuffle_1toN[k])] != BLACK
                || k == left - 1);
    }

    for (k = 0; k < left; ++k) {
        assert (state->grid[rotate(shuffle_1toN[k])] == BLACK);
        state->grid[shuffle_1toN[k]] = EMPTY;
    }
    for (k = left; k < right; ++k) {
        assert (state->grid[rotate(shuffle_1toN[k])] != BLACK);
        assert (state->grid[shuffle_1toN[k]] != BLACK);
    }
    for (k = right; k < n; ++k) {
        assert (state->grid[shuffle_1toN[k]] == BLACK);
        state->grid[shuffle_1toN[k]] = EMPTY;
    }

    clues_removed = (left - 0) + (n - right);

    dupstate = dup_game(state);
    buf = solve_internal(dupstate, move_buffer, DIFF_RECURSION - 1);
    free_game(dupstate);
    if (buf - move_buffer < clues_removed) {
        /* branch prediction: I don't think I'll go here */
        clues_removed = -1;
        goto ret;
    }

    for (k = left; k < right; ++k) {
        const int i = shuffle_1toN[k], j = rotate(i);
        int const clue = state->grid[i], clue_rot = state->grid[j];
        if (clue == BLACK) continue;
        state->grid[i] = state->grid[j] = EMPTY;
        dupstate = dup_game(state);
        buf = solve_internal(dupstate, move_buffer, DIFF_RECURSION - 1);
        free_game(dupstate);
        clues_removed += 2 - (i == j);
        /* if i is the center square, then i == (j = rotate(i))
         * when i and j are one, removing i and j removes only one */
        if (buf - move_buffer == clues_removed) continue;
        /* if the solver is sound, refilling all removed clues means
         * we have filled all squares, i.e. solved the puzzle. */
        state->grid[i] = clue;
        state->grid[j] = clue_rot;
        clues_removed -= 2 - (i == j);
    }
    
ret:
    sfree(move_buffer);
    return clues_removed;
}