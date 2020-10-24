static int check_completion(game_state *state)
{
    grid *g = state->game_grid;
    int *dsf;
    int num_faces = g->num_faces;
    int i;
    int infinite_area, finite_area;
    int loops_found = 0;
    int found_edge_not_in_loop = FALSE;

    memset(state->line_errors, 0, g->num_edges);

    /* LL implementation of SGT's idea:
     * A loop will partition the grid into an inside and an outside.
     * If there is more than one loop, the grid will be partitioned into
     * even more distinct regions.  We can therefore track equivalence of
     * faces, by saying that two faces are equivalent when there is a non-YES
     * edge between them.
     * We could keep track of the number of connected components, by counting
     * the number of dsf-merges that aren't no-ops.
     * But we're only interested in 3 separate cases:
     * no loops, one loop, more than one loop.
     *
     * No loops: all faces are equivalent to the infinite face.
     * One loop: only two equivalence classes - finite and infinite.
     * >= 2 loops: there are 2 distinct finite regions.
     *
     * So we simply make two passes through all the edges.
     * In the first pass, we dsf-merge the two faces bordering each non-YES
     * edge.
     * In the second pass, we look for YES-edges bordering:
     * a) two non-equivalent faces.
     * b) two non-equivalent faces, and one of them is part of a different
     *    finite area from the first finite area we've seen.
     *
     * An occurrence of a) means there is at least one loop.
     * An occurrence of b) means there is more than one loop.
     * Edges satisfying a) are marked as errors.
     *
     * While we're at it, we set a flag if we find a YES edge that is not
     * part of a loop.
     * This information will help decide, if there's a single loop, whether it
     * is a candidate for being a solution (that is, all YES edges are part of
     * this loop).
     *
     * If there is a candidate loop, we then go through all clues and check
     * they are all satisfied.  If so, we have found a solution and we can
     * unmark all line_errors.
     */
    
    /* Infinite face is at the end - its index is num_faces.
     * This macro is just to make this obvious! */
    #define INF_FACE num_faces
    dsf = snewn(num_faces + 1, int);
    dsf_init(dsf, num_faces + 1);
    
    /* First pass */
    for (i = 0; i < g->num_edges; i++) {
        grid_edge *e = g->edges + i;
        int f1 = e->face1 ? e->face1 - g->faces : INF_FACE;
        int f2 = e->face2 ? e->face2 - g->faces : INF_FACE;
        if (state->lines[i] != LINE_YES)
            dsf_merge(dsf, f1, f2);
    }
    
    /* Second pass */
    infinite_area = dsf_canonify(dsf, INF_FACE);
    finite_area = -1;
    for (i = 0; i < g->num_edges; i++) {
        grid_edge *e = g->edges + i;
        int f1 = e->face1 ? e->face1 - g->faces : INF_FACE;
        int can1 = dsf_canonify(dsf, f1);
        int f2 = e->face2 ? e->face2 - g->faces : INF_FACE;
        int can2 = dsf_canonify(dsf, f2);
        if (state->lines[i] != LINE_YES) continue;

        if (can1 == can2) {
            /* Faces are equivalent, so this edge not part of a loop */
            found_edge_not_in_loop = TRUE;
            continue;
        }
        state->line_errors[i] = TRUE;
        if (loops_found == 0) loops_found = 1;

        /* Don't bother with further checks if we've already found 2 loops */
        if (loops_found == 2) continue;

        if (finite_area == -1) {
            /* Found our first finite area */
            if (can1 != infinite_area)
                finite_area = can1;
            else
                finite_area = can2;
        }

        /* Have we found a second area? */
        if (finite_area != -1) {
            if (can1 != infinite_area && can1 != finite_area) {
                loops_found = 2;
                continue;
            }
            if (can2 != infinite_area && can2 != finite_area) {
                loops_found = 2;
            }
        }
    }

/*
    printf("loops_found = %d\n", loops_found);
    printf("found_edge_not_in_loop = %s\n",
        found_edge_not_in_loop ? "TRUE" : "FALSE");
*/

    sfree(dsf); /* No longer need the dsf */
    
    /* Have we found a candidate loop? */
    if (loops_found == 1 && !found_edge_not_in_loop) {
        /* Yes, so check all clues are satisfied */
        int found_clue_violation = FALSE;
        for (i = 0; i < num_faces; i++) {
            int c = state->clues[i];
            if (c >= 0) {
                if (face_order(state, i, LINE_YES) != c) {
                    found_clue_violation = TRUE;
                    break;
                }
            }
        }
        
        if (!found_clue_violation) {
            /* The loop is good */
            memset(state->line_errors, 0, g->num_edges);
            return TRUE; /* No need to bother checking for dot violations */
        }
    }

    /* Check for dot violations */
    for (i = 0; i < g->num_dots; i++) {
        int yes = dot_order(state, i, LINE_YES);
        int unknown = dot_order(state, i, LINE_UNKNOWN);
        if ((yes == 1 && unknown == 0) || (yes >= 3)) {
            /* violation, so mark all YES edges as errors */
            grid_dot *d = g->dots + i;
            int j;
            for (j = 0; j < d->order; j++) {
                int e = d->edges[j] - g->edges;
                if (state->lines[e] == LINE_YES)
                    state->line_errors[e] = TRUE;
            }
        }
    }
    return FALSE;
}