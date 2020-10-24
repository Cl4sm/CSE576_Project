static solver_state *new_solver_state(const game_state *state, int diff) {
    int i;
    int num_dots = state->game_grid->num_dots;
    int num_faces = state->game_grid->num_faces;
    int num_edges = state->game_grid->num_edges;
    solver_state *ret = snew(solver_state);

    ret->state = dup_game(state);

    ret->solver_status = SOLVER_INCOMPLETE;
    ret->diff = diff;

    ret->dotdsf = snew_dsf(num_dots);
    ret->looplen = snewn(num_dots, int);

    for (i = 0; i < num_dots; i++) {
        ret->looplen[i] = 1;
    }

    ret->dot_solved = snewn(num_dots, char);
    ret->face_solved = snewn(num_faces, char);
    memset(ret->dot_solved, FALSE, num_dots);
    memset(ret->face_solved, FALSE, num_faces);

    ret->dot_yes_count = snewn(num_dots, char);
    memset(ret->dot_yes_count, 0, num_dots);
    ret->dot_no_count = snewn(num_dots, char);
    memset(ret->dot_no_count, 0, num_dots);
    ret->face_yes_count = snewn(num_faces, char);
    memset(ret->face_yes_count, 0, num_faces);
    ret->face_no_count = snewn(num_faces, char);
    memset(ret->face_no_count, 0, num_faces);

    if (diff < DIFF_NORMAL) {
        ret->dlines = NULL;
    } else {
        ret->dlines = snewn(2*num_edges, char);
        memset(ret->dlines, 0, 2*num_edges);
    }

    if (diff < DIFF_HARD) {
        ret->linedsf = NULL;
    } else {
        ret->linedsf = snew_dsf(state->game_grid->num_edges);
    }

    return ret;
}