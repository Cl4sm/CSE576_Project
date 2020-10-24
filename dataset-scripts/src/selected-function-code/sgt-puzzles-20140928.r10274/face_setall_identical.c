static int face_setall_identical(solver_state *sstate, int face_index,
                                 enum line_state line_new)
{
    /* can[dir] contains the canonical line associated with the line in
     * direction dir from the square in question.  Similarly inv[dir] is
     * whether or not the line in question is inverse to its canonical
     * element. */
    int retval = FALSE;
    game_state *state = sstate->state;
    grid *g = state->game_grid;
    grid_face *f = g->faces + face_index;
    int N = f->order;
    int i, j;
    int can1, can2, inv1, inv2;

    for (i = 0; i < N; i++) {
        int line1_index = f->edges[i] - g->edges;
        if (state->lines[line1_index] != LINE_UNKNOWN)
            continue;
        for (j = i + 1; j < N; j++) {
            int line2_index = f->edges[j] - g->edges;
            if (state->lines[line2_index] != LINE_UNKNOWN)
                continue;

            /* Found two UNKNOWNS */
            can1 = edsf_canonify(sstate->linedsf, line1_index, &inv1);
            can2 = edsf_canonify(sstate->linedsf, line2_index, &inv2);
            if (can1 == can2 && inv1 == inv2) {
                solver_set_line(sstate, line1_index, line_new);
                solver_set_line(sstate, line2_index, line_new);
            }
        }
    }
    return retval;
}