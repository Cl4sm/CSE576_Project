static int linedsf_deductions(solver_state *sstate)
{
    game_state *state = sstate->state;
    grid *g = state->game_grid;
    char *dlines = sstate->dlines;
    int i;
    int diff = DIFF_MAX;
    int diff_tmp;

    /* ------ Face deductions ------ */

    /* A fully-general linedsf deduction seems overly complicated
     * (I suspect the problem is NP-complete, though in practice it might just
     * be doable because faces are limited in size).
     * For simplicity, we only consider *pairs* of LINE_UNKNOWNS that are
     * known to be identical.  If setting them both to YES (or NO) would break
     * the clue, set them to NO (or YES). */

    for (i = 0; i < g->num_faces; i++) {
        int N, yes, no, unknown;
        int clue;

        if (sstate->face_solved[i])
            continue;
        clue = state->clues[i];
        if (clue < 0)
            continue;

        N = g->faces[i].order;
        yes = sstate->face_yes_count[i];
        if (yes + 1 == clue) {
            if (face_setall_identical(sstate, i, LINE_NO))
                diff = min(diff, DIFF_EASY);
        }
        no = sstate->face_no_count[i];
        if (no + 1 == N - clue) {
            if (face_setall_identical(sstate, i, LINE_YES))
                diff = min(diff, DIFF_EASY);
        }

        /* Reload YES count, it might have changed */
        yes = sstate->face_yes_count[i];
        unknown = N - no - yes;

        /* Deductions with small number of LINE_UNKNOWNs, based on overall
         * parity of lines. */
        diff_tmp = parity_deductions(sstate, g->faces[i].edges,
                                     (clue - yes) % 2, unknown);
        diff = min(diff, diff_tmp);
    }

    /* ------ Dot deductions ------ */
    for (i = 0; i < g->num_dots; i++) {
        grid_dot *d = g->dots + i;
        int N = d->order;
        int j;
        int yes, no, unknown;
        /* Go through dlines, and do any dline<->linedsf deductions wherever
         * we find two UNKNOWNS. */
        for (j = 0; j < N; j++) {
            int dline_index = dline_index_from_dot(g, d, j);
            int line1_index;
            int line2_index;
            int can1, can2, inv1, inv2;
            int j2;
            line1_index = d->edges[j] - g->edges;
            if (state->lines[line1_index] != LINE_UNKNOWN)
                continue;
            j2 = j + 1;
            if (j2 == N) j2 = 0;
            line2_index = d->edges[j2] - g->edges;
            if (state->lines[line2_index] != LINE_UNKNOWN)
                continue;
            /* Infer dline flags from linedsf */
            can1 = edsf_canonify(sstate->linedsf, line1_index, &inv1);
            can2 = edsf_canonify(sstate->linedsf, line2_index, &inv2);
            if (can1 == can2 && inv1 != inv2) {
                /* These are opposites, so set dline atmostone/atleastone */
                if (set_atmostone(dlines, dline_index))
                    diff = min(diff, DIFF_NORMAL);
                if (set_atleastone(dlines, dline_index))
                    diff = min(diff, DIFF_NORMAL);
                continue;
            }
            /* Infer linedsf from dline flags */
            if (is_atmostone(dlines, dline_index)
		&& is_atleastone(dlines, dline_index)) {
                if (merge_lines(sstate, line1_index, line2_index, 1))
                    diff = min(diff, DIFF_HARD);
            }
        }

        /* Deductions with small number of LINE_UNKNOWNs, based on overall
         * parity of lines. */
        yes = sstate->dot_yes_count[i];
        no = sstate->dot_no_count[i];
        unknown = N - yes - no;
        diff_tmp = parity_deductions(sstate, d->edges,
                                     yes % 2, unknown);
        diff = min(diff, diff_tmp);
    }

    /* ------ Edge dsf deductions ------ */

    /* If the state of a line is known, deduce the state of its canonical line
     * too, and vice versa. */
    for (i = 0; i < g->num_edges; i++) {
        int can, inv;
        enum line_state s;
        can = edsf_canonify(sstate->linedsf, i, &inv);
        if (can == i)
            continue;
        s = sstate->state->lines[can];
        if (s != LINE_UNKNOWN) {
            if (solver_set_line(sstate, i, inv ? OPP(s) : s))
                diff = min(diff, DIFF_EASY);
        } else {
            s = sstate->state->lines[i];
            if (s != LINE_UNKNOWN) {
                if (solver_set_line(sstate, can, inv ? OPP(s) : s))
                    diff = min(diff, DIFF_EASY);
            }
        }
    }

    return diff;
}