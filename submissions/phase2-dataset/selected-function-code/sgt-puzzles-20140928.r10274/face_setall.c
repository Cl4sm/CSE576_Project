static int face_setall(solver_state *sstate, int face,
                       char old_type, char new_type)
{
    int retval = FALSE, r;
    game_state *state = sstate->state;
    grid *g;
    grid_face *f;
    int i;

    if (old_type == new_type)
        return FALSE;

    g = state->game_grid;
    f = g->faces + face;

    for (i = 0; i < f->order; i++) {
        int line_index = f->edges[i] - g->edges;
        if (state->lines[line_index] == old_type) {
            r = solver_set_line(sstate, line_index, new_type);
            assert(r == TRUE);
            retval = TRUE;
        }
    }
    return retval;
}