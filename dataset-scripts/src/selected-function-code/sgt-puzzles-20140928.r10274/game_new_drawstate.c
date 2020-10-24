static game_drawstate *game_new_drawstate(drawing *dr, const game_state *state)
{
    struct game_drawstate *ds = snew(struct game_drawstate);
    int i;

    memset(ds, 0, sizeof(struct game_drawstate));

    ds->guesses = snewn(state->params.nguesses, pegrow);
    ds->nguesses = state->params.nguesses;
    for (i = 0; i < state->params.nguesses; i++) {
        ds->guesses[i] = new_pegrow(state->params.npegs);
        invalidate_pegrow(ds->guesses[i]);
    }
    ds->solution = new_pegrow(state->params.npegs);
    invalidate_pegrow(ds->solution);
    ds->colours = new_pegrow(state->params.ncolours);
    invalidate_pegrow(ds->colours);

    ds->hintw = (state->params.npegs+1)/2; /* must round up */

    ds->blit_peg = NULL;

    return ds;
}