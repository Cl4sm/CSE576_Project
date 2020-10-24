static int merge_dots(solver_state *sstate, int edge_index)
{
    int i, j, len;
    grid *g = sstate->state->game_grid;
    grid_edge *e = g->edges + edge_index;

    i = e->dot1 - g->dots;
    j = e->dot2 - g->dots;

    i = dsf_canonify(sstate->dotdsf, i);
    j = dsf_canonify(sstate->dotdsf, j);

    if (i == j) {
        return TRUE;
    } else {
        len = sstate->looplen[i] + sstate->looplen[j];
        dsf_merge(sstate->dotdsf, i, j);
        i = dsf_canonify(sstate->dotdsf, i);
        sstate->looplen[i] = len;
        return FALSE;
    }
}