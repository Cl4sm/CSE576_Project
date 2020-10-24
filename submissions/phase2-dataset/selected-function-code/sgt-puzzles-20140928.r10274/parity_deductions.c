static int parity_deductions(solver_state *sstate,
    grid_edge **edge_list, /* Edge list (from a face or a dot) */
    int total_parity, /* Expected number of YESs modulo 2 (either 0 or 1) */
    int unknown_count)
{
    game_state *state = sstate->state;
    int diff = DIFF_MAX;
    int *linedsf = sstate->linedsf;

    if (unknown_count == 2) {
        /* Lines are known alike/opposite, depending on inv. */
        int e[2];
        find_unknowns(state, edge_list, 2, e);
        if (merge_lines(sstate, e[0], e[1], total_parity))
            diff = min(diff, DIFF_HARD);
    } else if (unknown_count == 3) {
        int e[3];
        int can[3]; /* canonical edges */
        int inv[3]; /* whether can[x] is inverse to e[x] */
        find_unknowns(state, edge_list, 3, e);
        can[0] = edsf_canonify(linedsf, e[0], inv);
        can[1] = edsf_canonify(linedsf, e[1], inv+1);
        can[2] = edsf_canonify(linedsf, e[2], inv+2);
        if (can[0] == can[1]) {
            if (solver_set_line(sstate, e[2], (total_parity^inv[0]^inv[1]) ?
				LINE_YES : LINE_NO))
                diff = min(diff, DIFF_EASY);
        }
        if (can[0] == can[2]) {
            if (solver_set_line(sstate, e[1], (total_parity^inv[0]^inv[2]) ?
				LINE_YES : LINE_NO))
                diff = min(diff, DIFF_EASY);
        }
        if (can[1] == can[2]) {
            if (solver_set_line(sstate, e[0], (total_parity^inv[1]^inv[2]) ?
				LINE_YES : LINE_NO))
                diff = min(diff, DIFF_EASY);
        }
    } else if (unknown_count == 4) {
        int e[4];
        int can[4]; /* canonical edges */
        int inv[4]; /* whether can[x] is inverse to e[x] */
        find_unknowns(state, edge_list, 4, e);
        can[0] = edsf_canonify(linedsf, e[0], inv);
        can[1] = edsf_canonify(linedsf, e[1], inv+1);
        can[2] = edsf_canonify(linedsf, e[2], inv+2);
        can[3] = edsf_canonify(linedsf, e[3], inv+3);
        if (can[0] == can[1]) {
            if (merge_lines(sstate, e[2], e[3], total_parity^inv[0]^inv[1]))
                diff = min(diff, DIFF_HARD);
        } else if (can[0] == can[2]) {
            if (merge_lines(sstate, e[1], e[3], total_parity^inv[0]^inv[2]))
                diff = min(diff, DIFF_HARD);
        } else if (can[0] == can[3]) {
            if (merge_lines(sstate, e[1], e[2], total_parity^inv[0]^inv[3]))
                diff = min(diff, DIFF_HARD);
        } else if (can[1] == can[2]) {
            if (merge_lines(sstate, e[0], e[3], total_parity^inv[1]^inv[2]))
                diff = min(diff, DIFF_HARD);
        } else if (can[1] == can[3]) {
            if (merge_lines(sstate, e[0], e[2], total_parity^inv[1]^inv[3]))
                diff = min(diff, DIFF_HARD);
        } else if (can[2] == can[3]) {
            if (merge_lines(sstate, e[0], e[1], total_parity^inv[2]^inv[3]))
                diff = min(diff, DIFF_HARD);
        }
    }
    return diff;
}