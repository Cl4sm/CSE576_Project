static void game_set_size(drawing *dr, game_drawstate *ds,
                          const game_params *params, int tilesize)
{
    int colh, guessh;

    ds->pegsz = tilesize;

    ds->hintsz = (int)((double)ds->pegsz * PEG_HINT);
    ds->gapsz  = (int)((double)ds->pegsz * PEG_GAP);
    ds->border = (int)((double)ds->pegsz * BORDER);

    ds->pegrad  = (ds->pegsz -1)/2; /* radius of peg to fit in pegsz (which is 2r+1) */
    ds->hintrad = (ds->hintsz-1)/2;

    colh = ((ds->pegsz + ds->gapsz) * params->ncolours) - ds->gapsz;
    guessh = ((ds->pegsz + ds->gapsz) * params->nguesses);      /* guesses */
    guessh += ds->gapsz + ds->pegsz;                            /* solution */

    game_compute_size(params, tilesize, &ds->w, &ds->h);
    ds->colx = ds->border;
    ds->coly = (ds->h - colh) / 2;

    ds->guessx = ds->solnx = ds->border + ds->pegsz * 2;     /* border + colours */
    ds->guessy = (ds->h - guessh) / 2;
    ds->solny = ds->guessy + ((ds->pegsz + ds->gapsz) * params->nguesses) + ds->gapsz;

    assert(ds->pegsz > 0);
    assert(!ds->blit_peg);             /* set_size is never called twice */
    ds->blit_peg = blitter_new(dr, ds->pegsz+2, ds->pegsz+2);
}