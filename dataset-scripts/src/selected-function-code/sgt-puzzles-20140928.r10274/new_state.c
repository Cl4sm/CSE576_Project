static game_state *new_state(const game_params *params)
{
    game_state *ret = snew(game_state);
    int wh = params->w * params->h, i;

    ret->w = params->w;
    ret->h = params->h;
    ret->allowloops = params->allowloops;
    ret->maxb = params->maxb;
    ret->params = *params;

    ret->grid = snewn(wh, grid_type);
    memset(ret->grid, 0, GRIDSZ(ret));
    ret->scratch = snewn(wh, grid_type);
    memset(ret->scratch, 0, GRIDSZ(ret));

    ret->wha = snewn(wh*N_WH_ARRAYS, char);
    memset(ret->wha, 0, wh*N_WH_ARRAYS*sizeof(char));

    ret->possv = ret->wha;
    ret->possh = ret->wha + wh;
    ret->lines = ret->wha + wh*2;
    ret->maxv = ret->wha + wh*3;
    ret->maxh = ret->wha + wh*4;

    memset(ret->maxv, ret->maxb, wh*sizeof(char));
    memset(ret->maxh, ret->maxb, wh*sizeof(char));

    ret->islands = NULL;
    ret->n_islands = 0;
    ret->n_islands_alloc = 0;

    ret->gridi = snewn(wh, struct island *);
    for (i = 0; i < wh; i++) ret->gridi[i] = NULL;

    ret->solved = ret->completed = 0;

    ret->solver = snew(struct solver_state);
    ret->solver->dsf = snew_dsf(wh);
    ret->solver->tmpdsf = snewn(wh, int);

    ret->solver->refcount = 1;

    return ret;
}