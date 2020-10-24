grid_edge *grid_nearest_edge(grid *g, int x, int y)
{
    grid_edge *best_edge;
    double best_distance = 0;
    int i;

    best_edge = NULL;

    for (i = 0; i < g->num_edges; i++) {
        grid_edge *e = &g->edges[i];
        long e2; /* squared length of edge */
        long a2, b2; /* squared lengths of other sides */
        double dist;

        /* See if edge e is eligible - the triangle must have acute angles
         * at the edge's dots.
         * Pythagoras formula h^2 = a^2 + b^2 detects right-angles,
         * so detect acute angles by testing for h^2 < a^2 + b^2 */
        e2 = SQ((long)e->dot1->x - (long)e->dot2->x) + SQ((long)e->dot1->y - (long)e->dot2->y);
        a2 = SQ((long)e->dot1->x - (long)x) + SQ((long)e->dot1->y - (long)y);
        b2 = SQ((long)e->dot2->x - (long)x) + SQ((long)e->dot2->y - (long)y);
        if (a2 >= e2 + b2) continue;
        if (b2 >= e2 + a2) continue;
         
        /* e is eligible so far.  Now check the edge is reasonably close
         * to where the user clicked.  Don't want to toggle an edge if the
         * click was way off the grid.
         * There is room for experimentation here.  We could check the
         * perpendicular distance is within a certain fraction of the length
         * of the edge.  That amounts to testing a rectangular region around
         * the edge.
         * Alternatively, we could check that the angle at the point is obtuse.
         * That would amount to testing a circular region with the edge as
         * diameter. */
        dist = point_line_distance((long)x, (long)y,
                                   (long)e->dot1->x, (long)e->dot1->y,
                                   (long)e->dot2->x, (long)e->dot2->y);
        /* Is dist more than half edge length ? */
        if (4 * SQ(dist) > e2)
            continue;

        if (best_edge == NULL || dist < best_distance) {
            best_edge = e;
            best_distance = dist;
        }
    }
    return best_edge;
}