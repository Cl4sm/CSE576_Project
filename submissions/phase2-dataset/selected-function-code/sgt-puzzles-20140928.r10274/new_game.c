static game_state *new_game(midend *me, const game_params *params,
                            const char *desc)
{
    game_state *state = snew(game_state);
    int i;
    const char *p;

    state->w = params->w;
    state->h = params->h;
    state->n = params->w * params->h;
    state->tiles = snewn(state->n, int);

    state->gap_pos = 0;
    p = desc;
    i = 0;
    for (i = 0; i < state->n; i++) {
        assert(*p);
        state->tiles[i] = atoi(p);
        if (state->tiles[i] == 0)
            state->gap_pos = i;
        while (*p && *p != ',')
            p++;
        if (*p) p++;                   /* eat comma */
    }
    assert(!*p);
    assert(state->tiles[state->gap_pos] == 0);

    state->completed = state->movecount = 0;
    state->used_solve = FALSE;

    return state;
}