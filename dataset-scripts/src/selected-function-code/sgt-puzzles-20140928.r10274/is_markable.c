static int is_markable(const game_params *params, pegrow pegs)
{
    int i, nset = 0, nrequired, ret = 0;
    pegrow colcount = new_pegrow(params->ncolours);

    nrequired = params->allow_blank ? 1 : params->npegs;

    for (i = 0; i < params->npegs; i++) {
        int c = pegs->pegs[i];
        if (c > 0) {
            colcount->pegs[c-1]++;
            nset++;
        }
    }
    if (nset < nrequired) goto done;

    if (!params->allow_multiple) {
        for (i = 0; i < params->ncolours; i++) {
            if (colcount->pegs[i] > 1) goto done;
        }
    }
    ret = 1;
done:
    free_pegrow(colcount);
    return ret;
}