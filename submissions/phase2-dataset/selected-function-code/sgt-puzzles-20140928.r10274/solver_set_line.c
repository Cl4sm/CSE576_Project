static int solver_set_line(solver_state *sstate, int i,
                           enum line_state line_new
#ifdef SHOW_WORKING
			   , const char *reason
#endif
			   )
{
    game_state *state = sstate->state;
    grid *g;
    grid_edge *e;

    assert(line_new != LINE_UNKNOWN);

    check_caches(sstate);

    if (state->lines[i] == line_new) {
        return FALSE; /* nothing changed */
    }
    state->lines[i] = line_new;

#ifdef SHOW_WORKING
    fprintf(stderr, "solver: set line [%d] to %s (%s)\n",
            i, line_new == LINE_YES ? "YES" : "NO",
            reason);
#endif

    g = state->game_grid;
    e = g->edges + i;

    /* Update the cache for both dots and both faces affected by this. */
    if (line_new == LINE_YES) {
        sstate->dot_yes_count[e->dot1 - g->dots]++;
        sstate->dot_yes_count[e->dot2 - g->dots]++;
        if (e->face1) {
            sstate->face_yes_count[e->face1 - g->faces]++;
        }
        if (e->face2) {
            sstate->face_yes_count[e->face2 - g->faces]++;
        }
    } else {
        sstate->dot_no_count[e->dot1 - g->dots]++;
        sstate->dot_no_count[e->dot2 - g->dots]++;
        if (e->face1) {
            sstate->face_no_count[e->face1 - g->faces]++;
        }
        if (e->face2) {
            sstate->face_no_count[e->face2 - g->faces]++;
        }
    }

    check_caches(sstate);
    return TRUE;
}