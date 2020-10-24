static int grid_edge_bydots_cmpfn(void *v1, void *v2)
{
    grid_edge *a = v1;
    grid_edge *b = v2;
    grid_dot *da, *db;

    /* Pointer subtraction is valid here, because all dots point into the
     * same dot-list (g->dots).
     * Edges are not "normalised" - the 2 dots could be stored in any order,
     * so we need to take this into account when comparing edges. */

    /* Compare first dots */
    da = (a->dot1 < a->dot2) ? a->dot1 : a->dot2;
    db = (b->dot1 < b->dot2) ? b->dot1 : b->dot2;
    if (da != db)
        return db - da;
    /* Compare last dots */
    da = (a->dot1 < a->dot2) ? a->dot2 : a->dot1;
    db = (b->dot1 < b->dot2) ? b->dot2 : b->dot1;
    if (da != db)
        return db - da;

    return 0;
}