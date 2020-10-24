static int loop_deductions(solver_state *sstate)
{
    int edgecount = 0, clues = 0, satclues = 0, sm1clues = 0;
    game_state *state = sstate->state;
    grid *g = state->game_grid;
    int shortest_chainlen = g->num_dots;
    int loop_found = FALSE;
    int dots_connected;
    int progress = FALSE;
    int i;

    /*
     * Go through the grid and update for all the new edges.
     * Since merge_dots() is idempotent, the simplest way to
     * do this is just to update for _all_ the edges.
     * Also, while we're here, we count the edges.
     */
    for (i = 0; i < g->num_edges; i++) {
        if (state->lines[i] == LINE_YES) {
            loop_found |= merge_dots(sstate, i);
            edgecount++;
        }
    }

    /*
     * Count the clues, count the satisfied clues, and count the
     * satisfied-minus-one clues.
     */
    for (i = 0; i < g->num_faces; i++) {
        int c = state->clues[i];
        if (c >= 0) {
            int o = sstate->face_yes_count[i];
            if (o == c)
                satclues++;
            else if (o == c-1)
                sm1clues++;
            clues++;
        }
    }

    for (i = 0; i < g->num_dots; ++i) {
        dots_connected =
            sstate->looplen[dsf_canonify(sstate->dotdsf, i)];
        if (dots_connected > 1)
            shortest_chainlen = min(shortest_chainlen, dots_connected);
    }

    assert(sstate->solver_status == SOLVER_INCOMPLETE);

    if (satclues == clues && shortest_chainlen == edgecount) {
        sstate->solver_status = SOLVER_SOLVED;
        /* This discovery clearly counts as progress, even if we haven't
         * just added any lines or anything */
        progress = TRUE;
        goto finished_loop_deductionsing;
    }

    /*
     * Now go through looking for LINE_UNKNOWN edges which
     * connect two dots that are already in the same
     * equivalence class. If we find one, test to see if the
     * loop it would create is a solution.
     */
    for (i = 0; i < g->num_edges; i++) {
        grid_edge *e = g->edges + i;
        int d1 = e->dot1 - g->dots;
        int d2 = e->dot2 - g->dots;
        int eqclass, val;
        if (state->lines[i] != LINE_UNKNOWN)
            continue;

        eqclass = dsf_canonify(sstate->dotdsf, d1);
        if (eqclass != dsf_canonify(sstate->dotdsf, d2))
            continue;

        val = LINE_NO;  /* loop is bad until proven otherwise */

        /*
         * This edge would form a loop. Next
         * question: how long would the loop be?
         * Would it equal the total number of edges
         * (plus the one we'd be adding if we added
         * it)?
         */
        if (sstate->looplen[eqclass] == edgecount + 1) {
            int sm1_nearby;

            /*
             * This edge would form a loop which
             * took in all the edges in the entire
             * grid. So now we need to work out
             * whether it would be a valid solution
             * to the puzzle, which means we have to
             * check if it satisfies all the clues.
             * This means that every clue must be
             * either satisfied or satisfied-minus-
             * 1, and also that the number of
             * satisfied-minus-1 clues must be at
             * most two and they must lie on either
             * side of this edge.
             */
            sm1_nearby = 0;
            if (e->face1) {
                int f = e->face1 - g->faces;
                int c = state->clues[f];
                if (c >= 0 && sstate->face_yes_count[f] == c - 1)
                    sm1_nearby++;
            }
            if (e->face2) {
                int f = e->face2 - g->faces;
                int c = state->clues[f];
                if (c >= 0 && sstate->face_yes_count[f] == c - 1)
                    sm1_nearby++;
            }
            if (sm1clues == sm1_nearby &&
		sm1clues + satclues == clues) {
                val = LINE_YES;  /* loop is good! */
            }
        }

        /*
         * Right. Now we know that adding this edge
         * would form a loop, and we know whether
         * that loop would be a viable solution or
         * not.
         *
         * If adding this edge produces a solution,
         * then we know we've found _a_ solution but
         * we don't know that it's _the_ solution -
         * if it were provably the solution then
         * we'd have deduced this edge some time ago
         * without the need to do loop detection. So
         * in this state we return SOLVER_AMBIGUOUS,
         * which has the effect that hitting Solve
         * on a user-provided puzzle will fill in a
         * solution but using the solver to
         * construct new puzzles won't consider this
         * a reasonable deduction for the user to
         * make.
         */
        progress = solver_set_line(sstate, i, val);
        assert(progress == TRUE);
        if (val == LINE_YES) {
            sstate->solver_status = SOLVER_AMBIGUOUS;
            goto finished_loop_deductionsing;
        }
    }

    finished_loop_deductionsing:
    return progress ? DIFF_EASY : DIFF_MAX;
}