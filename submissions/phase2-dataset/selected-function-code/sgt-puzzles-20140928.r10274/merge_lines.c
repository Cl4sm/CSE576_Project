static int merge_lines(solver_state *sstate, int i, int j, int inverse
#ifdef SHOW_WORKING
                       , const char *reason
#endif
		       )
{
    int inv_tmp;

    assert(i < sstate->state->game_grid->num_edges);
    assert(j < sstate->state->game_grid->num_edges);

    i = edsf_canonify(sstate->linedsf, i, &inv_tmp);
    inverse ^= inv_tmp;
    j = edsf_canonify(sstate->linedsf, j, &inv_tmp);
    inverse ^= inv_tmp;

    edsf_merge(sstate->linedsf, i, j, inverse);

#ifdef SHOW_WORKING
    if (i != j) {
        fprintf(stderr, "%s [%d] [%d] %s(%s)\n",
                __FUNCTION__, i, j,
                inverse ? "inverse " : "", reason);
    }
#endif
    return (i != j);
}