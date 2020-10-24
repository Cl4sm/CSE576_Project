static solver_state *dup_solver_state(const solver_state *sstate) {
    game_state *state = sstate->state;
    int num_dots = state->game_grid->num_dots;
    int num_faces = state->game_grid->num_faces;
    int num_edges = state->game_grid->num_edges;
    solver_state *ret = snew(solver_state);

    ret->state = state = dup_game(sstate->state);

    ret->solver_status = sstate->solver_status;
    ret->diff = sstate->diff;

    ret->dotdsf = snewn(num_dots, int);
    ret->looplen = snewn(num_dots, int);
    memcpy(ret->dotdsf, sstate->dotdsf,
           num_dots * sizeof(int));
    memcpy(ret->looplen, sstate->looplen,
           num_dots * sizeof(int));

    ret->dot_solved = snewn(num_dots, char);
    ret->face_solved = snewn(num_faces, char);
    memcpy(ret->dot_solved, sstate->dot_solved, num_dots);
    memcpy(ret->face_solved, sstate->face_solved, num_faces);

    ret->dot_yes_count = snewn(num_dots, char);
    memcpy(ret->dot_yes_count, sstate->dot_yes_count, num_dots);
    ret->dot_no_count = snewn(num_dots, char);
    memcpy(ret->dot_no_count, sstate->dot_no_count, num_dots);

    ret->face_yes_count = snewn(num_faces, char);
    memcpy(ret->face_yes_count, sstate->face_yes_count, num_faces);
    ret->face_no_count = snewn(num_faces, char);
    memcpy(ret->face_no_count, sstate->face_no_count, num_faces);

    if (sstate->dlines) {
        ret->dlines = snewn(2*num_edges, char);
        memcpy(ret->dlines, sstate->dlines,
               2*num_edges);
    } else {
        ret->dlines = NULL;
    }

    if (sstate->linedsf) {
        ret->linedsf = snewn(num_edges, int);
        memcpy(ret->linedsf, sstate->linedsf,
               num_edges * sizeof(int));
    } else {
        ret->linedsf = NULL;
    }

    return ret;
}