static int dline_deductions(solver_state *sstate)
{
    game_state *state = sstate->state;
    grid *g = state->game_grid;
    char *dlines = sstate->dlines;
    int i;
    int diff = DIFF_MAX;

    /* ------ Face deductions ------ */

    /* Given a set of dline atmostone/atleastone constraints, need to figure
     * out if we can deduce any further info.  For more general faces than
     * squares, this turns out to be a tricky problem.
     * The approach taken here is to define (per face) NxN matrices:
     * "maxs" and "mins".
     * The entries maxs(j,k) and mins(j,k) define the upper and lower limits
     * for the possible number of edges that are YES between positions j and k
     * going clockwise around the face.  Can think of j and k as marking dots
     * around the face (recall the labelling scheme: edge0 joins dot0 to dot1,
     * edge1 joins dot1 to dot2 etc).
     * Trivially, mins(j,j) = maxs(j,j) = 0, and we don't even bother storing
     * these.  mins(j,j+1) and maxs(j,j+1) are determined by whether edge{j}
     * is YES, NO or UNKNOWN.  mins(j,j+2) and maxs(j,j+2) are related to
     * the dline atmostone/atleastone status for edges j and j+1.
     *
     * Then we calculate the remaining entries recursively.  We definitely
     * know that
     * mins(j,k) >= { mins(j,u) + mins(u,k) } for any u between j and k.
     * This is because any valid placement of YESs between j and k must give
     * a valid placement between j and u, and also between u and k.
     * I believe it's sufficient to use just the two values of u:
     * j+1 and j+2.  Seems to work well in practice - the bounds we compute
     * are rigorous, even if they might not be best-possible.
     *
     * Once we have maxs and mins calculated, we can make inferences about
     * each dline{j,j+1} by looking at the possible complementary edge-counts
     * mins(j+2,j) and maxs(j+2,j) and comparing these with the face clue.
     * As well as dlines, we can make similar inferences about single edges.
     * For example, consider a pentagon with clue 3, and we know at most one
     * of (edge0, edge1) is YES, and at most one of (edge2, edge3) is YES.
     * We could then deduce edge4 is YES, because maxs(0,4) would be 2, so
     * that final edge would have to be YES to make the count up to 3.
     */

    /* Much quicker to allocate arrays on the stack than the heap, so
     * define the largest possible face size, and base our array allocations
     * on that.  We check this with an assertion, in case someone decides to
     * make a grid which has larger faces than this.  Note, this algorithm
     * could get quite expensive if there are many large faces. */
#define MAX_FACE_SIZE 12

    for (i = 0; i < g->num_faces; i++) {
        int maxs[MAX_FACE_SIZE][MAX_FACE_SIZE];
        int mins[MAX_FACE_SIZE][MAX_FACE_SIZE];
        grid_face *f = g->faces + i;
        int N = f->order;
        int j,m;
        int clue = state->clues[i];
        assert(N <= MAX_FACE_SIZE);
        if (sstate->face_solved[i])
            continue;
        if (clue < 0) continue;

        /* Calculate the (j,j+1) entries */
        for (j = 0; j < N; j++) {
            int edge_index = f->edges[j] - g->edges;
            int dline_index;
            enum line_state line1 = state->lines[edge_index];
            enum line_state line2;
            int tmp;
            int k = j + 1;
            if (k >= N) k = 0;
            maxs[j][k] = (line1 == LINE_NO) ? 0 : 1;
            mins[j][k] = (line1 == LINE_YES) ? 1 : 0;
            /* Calculate the (j,j+2) entries */
            dline_index = dline_index_from_face(g, f, k);
            edge_index = f->edges[k] - g->edges;
            line2 = state->lines[edge_index];
            k++;
            if (k >= N) k = 0;

            /* max */
            tmp = 2;
            if (line1 == LINE_NO) tmp--;
            if (line2 == LINE_NO) tmp--;
            if (tmp == 2 && is_atmostone(dlines, dline_index))
                tmp = 1;
            maxs[j][k] = tmp;

            /* min */
            tmp = 0;
            if (line1 == LINE_YES) tmp++;
            if (line2 == LINE_YES) tmp++;
            if (tmp == 0 && is_atleastone(dlines, dline_index))
                tmp = 1;
            mins[j][k] = tmp;
        }

        /* Calculate the (j,j+m) entries for m between 3 and N-1 */
        for (m = 3; m < N; m++) {
            for (j = 0; j < N; j++) {
                int k = j + m;
                int u = j + 1;
                int v = j + 2;
                int tmp;
                if (k >= N) k -= N;
                if (u >= N) u -= N;
                if (v >= N) v -= N;
                maxs[j][k] = maxs[j][u] + maxs[u][k];
                mins[j][k] = mins[j][u] + mins[u][k];
                tmp = maxs[j][v] + maxs[v][k];
                maxs[j][k] = min(maxs[j][k], tmp);
                tmp = mins[j][v] + mins[v][k];
                mins[j][k] = max(mins[j][k], tmp);
            }
        }

        /* See if we can make any deductions */
        for (j = 0; j < N; j++) {
            int k;
            grid_edge *e = f->edges[j];
            int line_index = e - g->edges;
            int dline_index;

            if (state->lines[line_index] != LINE_UNKNOWN)
                continue;
            k = j + 1;
            if (k >= N) k = 0;

            /* minimum YESs in the complement of this edge */
            if (mins[k][j] > clue) {
                sstate->solver_status = SOLVER_MISTAKE;
                return DIFF_EASY;
            }
            if (mins[k][j] == clue) {
                /* setting this edge to YES would make at least
                 * (clue+1) edges - contradiction */
                solver_set_line(sstate, line_index, LINE_NO);
                diff = min(diff, DIFF_EASY);
            }
            if (maxs[k][j] < clue - 1) {
                sstate->solver_status = SOLVER_MISTAKE;
                return DIFF_EASY;
            }
            if (maxs[k][j] == clue - 1) {
                /* Only way to satisfy the clue is to set edge{j} as YES */
                solver_set_line(sstate, line_index, LINE_YES);
                diff = min(diff, DIFF_EASY);
            }

            /* More advanced deduction that allows propagation along diagonal
             * chains of faces connected by dots, for example, 3-2-...-2-3
             * in square grids. */
            if (sstate->diff >= DIFF_TRICKY) {
                /* Now see if we can make dline deduction for edges{j,j+1} */
                e = f->edges[k];
                if (state->lines[e - g->edges] != LINE_UNKNOWN)
                    /* Only worth doing this for an UNKNOWN,UNKNOWN pair.
                     * Dlines where one of the edges is known, are handled in the
                     * dot-deductions */
                    continue;
    
                dline_index = dline_index_from_face(g, f, k);
                k++;
                if (k >= N) k = 0;
    
                /* minimum YESs in the complement of this dline */
                if (mins[k][j] > clue - 2) {
                    /* Adding 2 YESs would break the clue */
                    if (set_atmostone(dlines, dline_index))
                        diff = min(diff, DIFF_NORMAL);
                }
                /* maximum YESs in the complement of this dline */
                if (maxs[k][j] < clue) {
                    /* Adding 2 NOs would mean not enough YESs */
                    if (set_atleastone(dlines, dline_index))
                        diff = min(diff, DIFF_NORMAL);
                }
            }
        }
    }

    if (diff < DIFF_NORMAL)
        return diff;

    /* ------ Dot deductions ------ */

    for (i = 0; i < g->num_dots; i++) {
        grid_dot *d = g->dots + i;
        int N = d->order;
        int yes, no, unknown;
        int j;
        if (sstate->dot_solved[i])
            continue;
        yes = sstate->dot_yes_count[i];
        no = sstate->dot_no_count[i];
        unknown = N - yes - no;

        for (j = 0; j < N; j++) {
            int k;
            int dline_index;
            int line1_index, line2_index;
            enum line_state line1, line2;
            k = j + 1;
            if (k >= N) k = 0;
            dline_index = dline_index_from_dot(g, d, j);
            line1_index = d->edges[j] - g->edges;
            line2_index = d->edges[k] - g->edges;
            line1 = state->lines[line1_index];
            line2 = state->lines[line2_index];

            /* Infer dline state from line state */
            if (line1 == LINE_NO || line2 == LINE_NO) {
                if (set_atmostone(dlines, dline_index))
                    diff = min(diff, DIFF_NORMAL);
            }
            if (line1 == LINE_YES || line2 == LINE_YES) {
                if (set_atleastone(dlines, dline_index))
                    diff = min(diff, DIFF_NORMAL);
            }
            /* Infer line state from dline state */
            if (is_atmostone(dlines, dline_index)) {
                if (line1 == LINE_YES && line2 == LINE_UNKNOWN) {
                    solver_set_line(sstate, line2_index, LINE_NO);
                    diff = min(diff, DIFF_EASY);
                }
                if (line2 == LINE_YES && line1 == LINE_UNKNOWN) {
                    solver_set_line(sstate, line1_index, LINE_NO);
                    diff = min(diff, DIFF_EASY);
                }
            }
            if (is_atleastone(dlines, dline_index)) {
                if (line1 == LINE_NO && line2 == LINE_UNKNOWN) {
                    solver_set_line(sstate, line2_index, LINE_YES);
                    diff = min(diff, DIFF_EASY);
                }
                if (line2 == LINE_NO && line1 == LINE_UNKNOWN) {
                    solver_set_line(sstate, line1_index, LINE_YES);
                    diff = min(diff, DIFF_EASY);
                }
            }
            /* Deductions that depend on the numbers of lines.
             * Only bother if both lines are UNKNOWN, otherwise the
             * easy-mode solver (or deductions above) would have taken
             * care of it. */
            if (line1 != LINE_UNKNOWN || line2 != LINE_UNKNOWN)
                continue;

            if (yes == 0 && unknown == 2) {
                /* Both these unknowns must be identical.  If we know
                 * atmostone or atleastone, we can make progress. */
                if (is_atmostone(dlines, dline_index)) {
                    solver_set_line(sstate, line1_index, LINE_NO);
                    solver_set_line(sstate, line2_index, LINE_NO);
                    diff = min(diff, DIFF_EASY);
                }
                if (is_atleastone(dlines, dline_index)) {
                    solver_set_line(sstate, line1_index, LINE_YES);
                    solver_set_line(sstate, line2_index, LINE_YES);
                    diff = min(diff, DIFF_EASY);
                }
            }
            if (yes == 1) {
                if (set_atmostone(dlines, dline_index))
                    diff = min(diff, DIFF_NORMAL);
                if (unknown == 2) {
                    if (set_atleastone(dlines, dline_index))
                        diff = min(diff, DIFF_NORMAL);
                }
            }

            /* More advanced deduction that allows propagation along diagonal
             * chains of faces connected by dots, for example: 3-2-...-2-3
             * in square grids. */
            if (sstate->diff >= DIFF_TRICKY) {
                /* If we have atleastone set for this dline, infer
                 * atmostone for each "opposite" dline (that is, each
                 * dline without edges in common with this one).
                 * Again, this test is only worth doing if both these
                 * lines are UNKNOWN.  For if one of these lines were YES,
                 * the (yes == 1) test above would kick in instead. */
                if (is_atleastone(dlines, dline_index)) {
                    int opp;
                    for (opp = 0; opp < N; opp++) {
                        int opp_dline_index;
                        if (opp == j || opp == j+1 || opp == j-1)
                            continue;
                        if (j == 0 && opp == N-1)
                            continue;
                        if (j == N-1 && opp == 0)
                            continue;
                        opp_dline_index = dline_index_from_dot(g, d, opp);
                        if (set_atmostone(dlines, opp_dline_index))
                            diff = min(diff, DIFF_NORMAL);
                    }
                    if (yes == 0 && is_atmostone(dlines, dline_index)) {
                        /* This dline has *exactly* one YES and there are no
                         * other YESs.  This allows more deductions. */
                        if (unknown == 3) {
                            /* Third unknown must be YES */
                            for (opp = 0; opp < N; opp++) {
                                int opp_index;
                                if (opp == j || opp == k)
                                    continue;
                                opp_index = d->edges[opp] - g->edges;
                                if (state->lines[opp_index] == LINE_UNKNOWN) {
                                    solver_set_line(sstate, opp_index,
                                                    LINE_YES);
                                    diff = min(diff, DIFF_EASY);
                                }
                            }
                        } else if (unknown == 4) {
                            /* Exactly one of opposite UNKNOWNS is YES.  We've
                             * already set atmostone, so set atleastone as
                             * well.
                             */
                            if (dline_set_opp_atleastone(sstate, d, j))
                                diff = min(diff, DIFF_NORMAL);
                        }
                    }
                }
            }
        }
    }
    return diff;
}