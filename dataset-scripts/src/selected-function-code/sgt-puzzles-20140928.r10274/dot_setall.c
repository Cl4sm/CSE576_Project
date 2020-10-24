static int dot_setall(solver_state *sstate, int dot,
		      char old_type, char new_type)
{
    int retval = FALSE, r;
    game_state *state = sstate->state;
    grid *g;
    grid_dot *d;
    int i;

    if (old_type == new_type)
        return FALSE;

    g = state->game_grid;
    d = g->dots + dot;

    for (i = 0; i < d->order; i++) {
        int line_index = d->edges[i] - g->edges;
        if (state->lines[line_index] == old_type) {
            r = solver_set_line(sstate, line_index, new_type);
            assert(r == TRUE);
            retval = TRUE;
        }
    }
    return retval;
}