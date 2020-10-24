static int trivial_deductions(solver_state *sstate)
{
    int i, current_yes, current_no;
    game_state *state = sstate->state;
    grid *g = state->game_grid;
    int diff = DIFF_MAX;

    /* Per-face deductions */
    for (i = 0; i < g->num_faces; i++) {
        grid_face *f = g->faces + i;

        if (sstate->face_solved[i])
            continue;

        current_yes = sstate->face_yes_count[i];
        current_no  = sstate->face_no_count[i];

        if (current_yes + current_no == f->order)  {
            sstate->face_solved[i] = TRUE;
            continue;
        }

        if (state->clues[i] < 0)
            continue;

        /*
         * This code checks whether the numeric clue on a face is so
         * large as to permit all its remaining LINE_UNKNOWNs to be
         * filled in as LINE_YES, or alternatively so small as to
         * permit them all to be filled in as LINE_NO.
         */

        if (state->clues[i] < current_yes) {
            sstate->solver_status = SOLVER_MISTAKE;
            return DIFF_EASY;
        }
        if (state->clues[i] == current_yes) {
            if (face_setall(sstate, i, LINE_UNKNOWN, LINE_NO))
                diff = min(diff, DIFF_EASY);
            sstate->face_solved[i] = TRUE;
            continue;
        }

        if (f->order - state->clues[i] < current_no) {
            sstate->solver_status = SOLVER_MISTAKE;
            return DIFF_EASY;
        }
        if (f->order - state->clues[i] == current_no) {
            if (face_setall(sstate, i, LINE_UNKNOWN, LINE_YES))
                diff = min(diff, DIFF_EASY);
            sstate->face_solved[i] = TRUE;
            continue;
        }

        if (f->order - state->clues[i] == current_no + 1 &&
            f->order - current_yes - current_no > 2) {
            /*
             * One small refinement to the above: we also look for any
             * adjacent pair of LINE_UNKNOWNs around the face with
             * some LINE_YES incident on it from elsewhere. If we find
             * one, then we know that pair of LINE_UNKNOWNs can't
             * _both_ be LINE_YES, and hence that pushes us one line
             * closer to being able to determine all the rest.
             */
            int j, k, e1, e2, e, d;

            for (j = 0; j < f->order; j++) {
                e1 = f->edges[j] - g->edges;
                e2 = f->edges[j+1 < f->order ? j+1 : 0] - g->edges;

                if (g->edges[e1].dot1 == g->edges[e2].dot1 ||
                    g->edges[e1].dot1 == g->edges[e2].dot2) {
                    d = g->edges[e1].dot1 - g->dots;
                } else {
                    assert(g->edges[e1].dot2 == g->edges[e2].dot1 ||
                           g->edges[e1].dot2 == g->edges[e2].dot2);
                    d = g->edges[e1].dot2 - g->dots;
                }

                if (state->lines[e1] == LINE_UNKNOWN &&
                    state->lines[e2] == LINE_UNKNOWN) {
                    for (k = 0; k < g->dots[d].order; k++) {
                        int e = g->dots[d].edges[k] - g->edges;
                        if (state->lines[e] == LINE_YES)
                            goto found;    /* multi-level break */
                    }
                }
            }
            continue;

          found:
            /*
             * If we get here, we've found such a pair of edges, and
             * they're e1 and e2.
             */
            for (j = 0; j < f->order; j++) {
                e = f->edges[j] - g->edges;
                if (state->lines[e] == LINE_UNKNOWN && e != e1 && e != e2) {
                    int r = solver_set_line(sstate, e, LINE_YES);
                    assert(r);
                    diff = min(diff, DIFF_EASY);
                }
            }
        }
    }

    check_caches(sstate);

    /* Per-dot deductions */
    for (i = 0; i < g->num_dots; i++) {
        grid_dot *d = g->dots + i;
        int yes, no, unknown;

        if (sstate->dot_solved[i])
            continue;

        yes = sstate->dot_yes_count[i];
        no = sstate->dot_no_count[i];
        unknown = d->order - yes - no;

        if (yes == 0) {
            if (unknown == 0) {
                sstate->dot_solved[i] = TRUE;
            } else if (unknown == 1) {
                dot_setall(sstate, i, LINE_UNKNOWN, LINE_NO);
                diff = min(diff, DIFF_EASY);
                sstate->dot_solved[i] = TRUE;
            }
        } else if (yes == 1) {
            if (unknown == 0) {
                sstate->solver_status = SOLVER_MISTAKE;
                return DIFF_EASY;
            } else if (unknown == 1) {
                dot_setall(sstate, i, LINE_UNKNOWN, LINE_YES);
                diff = min(diff, DIFF_EASY);
            }
        } else if (yes == 2) {
            if (unknown > 0) {
                dot_setall(sstate, i, LINE_UNKNOWN, LINE_NO);
                diff = min(diff, DIFF_EASY);
            }
            sstate->dot_solved[i] = TRUE;
        } else {
            sstate->solver_status = SOLVER_MISTAKE;
            return DIFF_EASY;
        }
    }

    check_caches(sstate);

    return diff;
}