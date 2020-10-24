static int dline_set_opp_atleastone(solver_state *sstate,
                                    grid_dot *d, int edge)
{
    game_state *state = sstate->state;
    grid *g = state->game_grid;
    int N = d->order;
    int opp, opp2;
    for (opp = 0; opp < N; opp++) {
        int opp_dline_index;
        if (opp == edge || opp == edge+1 || opp == edge-1)
            continue;
        if (opp == 0 && edge == N-1)
            continue;
        if (opp == N-1 && edge == 0)
            continue;
        opp2 = opp + 1;
        if (opp2 == N) opp2 = 0;
        /* Check if opp, opp2 point to LINE_UNKNOWNs */
        if (state->lines[d->edges[opp] - g->edges] != LINE_UNKNOWN)
            continue;
        if (state->lines[d->edges[opp2] - g->edges] != LINE_UNKNOWN)
            continue;
        /* Found opposite UNKNOWNS and they're next to each other */
        opp_dline_index = dline_index_from_dot(g, d, opp);
        return set_atleastone(sstate->dlines, opp_dline_index);
    }
    return FALSE;
}