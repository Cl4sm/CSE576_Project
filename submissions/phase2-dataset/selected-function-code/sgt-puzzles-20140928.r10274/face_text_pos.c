static void face_text_pos(const game_drawstate *ds, const grid *g,
                          grid_face *f, int *xret, int *yret)
{
    int faceindex = f - g->faces;

    /*
     * Return the cached position for this face, if we've already
     * worked it out.
     */
    if (ds->textx[faceindex] >= 0) {
        *xret = ds->textx[faceindex];
        *yret = ds->texty[faceindex];
        return;
    }

    /*
     * Otherwise, use the incentre computed by grid.c and convert it
     * to screen coordinates.
     */
    grid_find_incentre(f);
    grid_to_screen(ds, g, f->ix, f->iy,
                   &ds->textx[faceindex], &ds->texty[faceindex]);

    *xret = ds->textx[faceindex];
    *yret = ds->texty[faceindex];
}