static game_state *dup_game(const game_state *state)
{
    game_state *ret = snew(game_state);

    ret->game_grid = state->game_grid;
    ret->game_grid->refcount++;

    ret->solved = state->solved;
    ret->cheated = state->cheated;

    ret->clues = snewn(state->game_grid->num_faces, signed char);
    memcpy(ret->clues, state->clues, state->game_grid->num_faces);

    ret->lines = snewn(state->game_grid->num_edges, char);
    memcpy(ret->lines, state->lines, state->game_grid->num_edges);

    ret->line_errors = snewn(state->game_grid->num_edges, unsigned char);
    memcpy(ret->line_errors, state->line_errors, state->game_grid->num_edges);

    ret->grid_type = state->grid_type;
    return ret;
}