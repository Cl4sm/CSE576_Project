static struct island *island_add(game_state *state, int x, int y, int count)
{
    struct island *is;
    int realloced = 0;

    assert(!(GRID(state,x,y) & G_ISLAND));
    GRID(state,x,y) |= G_ISLAND;

    state->n_islands++;
    if (state->n_islands > state->n_islands_alloc) {
        state->n_islands_alloc = state->n_islands * 2;
        state->islands =
            sresize(state->islands, state->n_islands_alloc, struct island);
        realloced = 1;
    }
    is = &state->islands[state->n_islands-1];

    memset(is, 0, sizeof(struct island));
    is->state = state;
    is->x = x;
    is->y = y;
    is->count = count;
    island_set_surrounds(is);

    if (realloced)
        fixup_islands_for_realloc(state);
    else
        INDEX(state, gridi, x, y) = is;

    return is;
}