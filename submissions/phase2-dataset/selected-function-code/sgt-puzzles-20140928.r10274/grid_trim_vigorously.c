static void grid_trim_vigorously(grid *g)
{
    int *dotpairs, *faces, *dots;
    int *dsf;
    int i, j, k, size, newfaces, newdots;

    /*
     * First construct a matrix in which each ordered pair of dots is
     * mapped to the index of the face in which those dots occur in
     * that order.
     */
    dotpairs = snewn(g->num_dots * g->num_dots, int);
    for (i = 0; i < g->num_dots; i++)
        for (j = 0; j < g->num_dots; j++)
            dotpairs[i*g->num_dots+j] = -1;
    for (i = 0; i < g->num_faces; i++) {
        grid_face *f = g->faces + i;
        int dot0 = f->dots[f->order-1] - g->dots;
        for (j = 0; j < f->order; j++) {
            int dot1 = f->dots[j] - g->dots;
            dotpairs[dot0 * g->num_dots + dot1] = i;
            dot0 = dot1;
        }
    }

    /*
     * Now we can identify landlocked dots: they're the ones all of
     * whose edges have a mirror-image counterpart in this matrix.
     */
    dots = snewn(g->num_dots, int);
    for (i = 0; i < g->num_dots; i++) {
        dots[i] = TRUE;
        for (j = 0; j < g->num_dots; j++) {
            if ((dotpairs[i*g->num_dots+j] >= 0) ^
                (dotpairs[j*g->num_dots+i] >= 0))
                dots[i] = FALSE;    /* non-duplicated edge: coastal dot */
        }
    }

    /*
     * Now identify connected pairs of landlocked dots, and form a dsf
     * unifying them.
     */
    dsf = snew_dsf(g->num_dots);
    for (i = 0; i < g->num_dots; i++)
        for (j = 0; j < i; j++)
            if (dots[i] && dots[j] &&
                dotpairs[i*g->num_dots+j] >= 0 &&
                dotpairs[j*g->num_dots+i] >= 0)
                dsf_merge(dsf, i, j);

    /*
     * Now look for the largest component.
     */
    size = 0;
    j = -1;
    for (i = 0; i < g->num_dots; i++) {
        int newsize;
        if (dots[i] && dsf_canonify(dsf, i) == i &&
            (newsize = dsf_size(dsf, i)) > size) {
            j = i;
            size = newsize;
        }
    }

    /*
     * Work out which faces we're going to keep (precisely those with
     * at least one dot in the same connected component as j) and
     * which dots (those required by any face we're keeping).
     *
     * At this point we reuse the 'dots' array to indicate the dots
     * we're keeping, rather than the ones that are landlocked.
     */
    faces = snewn(g->num_faces, int);
    for (i = 0; i < g->num_faces; i++)
        faces[i] = 0;
    for (i = 0; i < g->num_dots; i++)
        dots[i] = 0;
    for (i = 0; i < g->num_faces; i++) {
        grid_face *f = g->faces + i;
        int keep = FALSE;
        for (k = 0; k < f->order; k++)
            if (dsf_canonify(dsf, f->dots[k] - g->dots) == j)
                keep = TRUE;
        if (keep) {
            faces[i] = TRUE;
            for (k = 0; k < f->order; k++)
                dots[f->dots[k]-g->dots] = TRUE;
        }
    }

    /*
     * Work out the new indices of those faces and dots, when we
     * compact the arrays containing them.
     */
    for (i = newfaces = 0; i < g->num_faces; i++)
        faces[i] = (faces[i] ? newfaces++ : -1);
    for (i = newdots = 0; i < g->num_dots; i++)
        dots[i] = (dots[i] ? newdots++ : -1);

    /*
     * Free the dynamically allocated 'dots' pointer lists in faces
     * we're going to discard.
     */
    for (i = 0; i < g->num_faces; i++)
        if (faces[i] < 0)
            sfree(g->faces[i].dots);

    /*
     * Go through and compact the arrays.
     */
    for (i = 0; i < g->num_dots; i++)
        if (dots[i] >= 0) {
            grid_dot *dnew = g->dots + dots[i], *dold = g->dots + i;
            *dnew = *dold;             /* structure copy */
        }
    for (i = 0; i < g->num_faces; i++)
        if (faces[i] >= 0) {
            grid_face *fnew = g->faces + faces[i], *fold = g->faces + i;
            *fnew = *fold;             /* structure copy */
            for (j = 0; j < fnew->order; j++) {
                /*
                 * Reindex the dots in this face.
                 */
                k = fnew->dots[j] - g->dots;
                fnew->dots[j] = g->dots + dots[k];
            }
        }
    g->num_faces = newfaces;
    g->num_dots = newdots;

    sfree(dotpairs);
    sfree(dsf);
    sfree(dots);
    sfree(faces);
}