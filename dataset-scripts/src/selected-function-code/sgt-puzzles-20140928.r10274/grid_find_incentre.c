void grid_find_incentre(grid_face *f)
{
    double xbest, ybest, bestdist;
    int i, j, k, m;
    grid_dot *edgedot1[3], *edgedot2[3];
    grid_dot *dots[3];
    int nedges, ndots;

    if (f->has_incentre)
        return;

    /*
     * Find the point in the polygon with the maximum distance to any
     * edge or corner.
     *
     * Such a point must exist which is in contact with at least three
     * edges and/or vertices. (Proof: if it's only in contact with two
     * edges and/or vertices, it can't even be at a _local_ maximum -
     * any such circle can always be expanded in some direction.) So
     * we iterate through all 3-subsets of the combined set of edges
     * and vertices; for each subset we generate one or two candidate
     * points that might be the incentre, and then we vet each one to
     * see if it's inside the polygon and what its maximum radius is.
     *
     * (There's one case which this algorithm will get noticeably
     * wrong, and that's when a continuum of equally good answers
     * exists due to parallel edges. Consider a long thin rectangle,
     * for instance, or a parallelogram. This algorithm will pick a
     * point near one end, and choose the end arbitrarily; obviously a
     * nicer point to choose would be in the centre. To fix this I
     * would have to introduce a special-case system which detected
     * parallel edges in advance, set aside all candidate points
     * generated using both edges in a parallel pair, and generated
     * some additional candidate points half way between them. Also,
     * of course, I'd have to cope with rounding error making such a
     * point look worse than one of its endpoints. So I haven't done
     * this for the moment, and will cross it if necessary when I come
     * to it.)
     *
     * We don't actually iterate literally over _edges_, in the sense
     * of grid_edge structures. Instead, we fill in edgedot1[] and
     * edgedot2[] with a pair of dots adjacent in the face's list of
     * vertices. This ensures that we get the edges in consistent
     * orientation, which we could not do from the grid structure
     * alone. (A moment's consideration of an order-3 vertex should
     * make it clear that if a notional arrow was written on each
     * edge, _at least one_ of the three faces bordering that vertex
     * would have to have the two arrows tip-to-tip or tail-to-tail
     * rather than tip-to-tail.)
     */
    nedges = ndots = 0;
    bestdist = 0;
    xbest = ybest = 0;

    for (i = 0; i+2 < 2*f->order; i++) {
        if (i < f->order) {
            edgedot1[nedges] = f->dots[i];
            edgedot2[nedges++] = f->dots[(i+1)%f->order];
        } else
            dots[ndots++] = f->dots[i - f->order];

        for (j = i+1; j+1 < 2*f->order; j++) {
            if (j < f->order) {
                edgedot1[nedges] = f->dots[j];
                edgedot2[nedges++] = f->dots[(j+1)%f->order];
            } else
                dots[ndots++] = f->dots[j - f->order];

            for (k = j+1; k < 2*f->order; k++) {
                double cx[2], cy[2];   /* candidate positions */
                int cn = 0;            /* number of candidates */

                if (k < f->order) {
                    edgedot1[nedges] = f->dots[k];
                    edgedot2[nedges++] = f->dots[(k+1)%f->order];
                } else
                    dots[ndots++] = f->dots[k - f->order];

                /*
                 * Find a point, or pair of points, equidistant from
                 * all the specified edges and/or vertices.
                 */
                if (nedges == 3) {
                    /*
                     * Three edges. This is a linear matrix equation:
                     * each row of the matrix represents the fact that
                     * the point (x,y) we seek is at distance r from
                     * that edge, and we solve three of those
                     * simultaneously to obtain x,y,r. (We ignore r.)
                     */
                    double matrix[9], vector[3], vector2[3];
                    int m;

                    for (m = 0; m < 3; m++) {
                        int x1 = edgedot1[m]->x, x2 = edgedot2[m]->x;
                        int y1 = edgedot1[m]->y, y2 = edgedot2[m]->y;
                        int dx = x2-x1, dy = y2-y1;

                        /*
                         * ((x,y) - (x1,y1)) . (dy,-dx) = r |(dx,dy)|
                         *
                         * => x dy - y dx - r |(dx,dy)| = (x1 dy - y1 dx)
                         */
                        matrix[3*m+0] = dy;
                        matrix[3*m+1] = -dx;
                        matrix[3*m+2] = -sqrt((double)dx*dx+(double)dy*dy);
                        vector[m] = (double)x1*dy - (double)y1*dx;
                    }

                    if (solve_3x3_matrix(matrix, vector, vector2)) {
                        cx[cn] = vector2[0];
                        cy[cn] = vector2[1];
                        cn++;
                    }
                } else if (nedges == 2) {
                    /*
                     * Two edges and a dot. This will end up in a
                     * quadratic equation.
                     *
                     * First, look at the two edges. Having our point
                     * be some distance r from both of them gives rise
                     * to a pair of linear equations in x,y,r of the
                     * form
                     *
                     *   (x-x1) dy - (y-y1) dx = r sqrt(dx^2+dy^2)
                     *
                     * We eliminate r between those equations to give
                     * us a single linear equation in x,y describing
                     * the locus of points equidistant from both lines
                     * - i.e. the angle bisector. 
                     *
                     * We then choose one of x,y to be a parameter t,
                     * and derive linear formulae for x,y,r in terms
                     * of t. This enables us to write down the
                     * circular equation (x-xd)^2+(y-yd)^2=r^2 as a
                     * quadratic in t; solving that and substituting
                     * in for x,y gives us two candidate points.
                     */
                    double eqs[2][4];  /* a,b,c,d : ax+by+cr=d */
                    double eq[3];      /* a,b,c: ax+by=c */
                    double xt[2], yt[2], rt[2]; /* a,b: {x,y,r}=at+b */
                    double q[3];                /* a,b,c: at^2+bt+c=0 */
                    double disc;

                    /* Find equations of the two input lines. */
                    for (m = 0; m < 2; m++) {
                        int x1 = edgedot1[m]->x, x2 = edgedot2[m]->x;
                        int y1 = edgedot1[m]->y, y2 = edgedot2[m]->y;
                        int dx = x2-x1, dy = y2-y1;

                        eqs[m][0] = dy;
                        eqs[m][1] = -dx;
                        eqs[m][2] = -sqrt(dx*dx+dy*dy);
                        eqs[m][3] = x1*dy - y1*dx;
                    }

                    /* Derive the angle bisector by eliminating r. */
                    eq[0] = eqs[0][0]*eqs[1][2] - eqs[1][0]*eqs[0][2];
                    eq[1] = eqs[0][1]*eqs[1][2] - eqs[1][1]*eqs[0][2];
                    eq[2] = eqs[0][3]*eqs[1][2] - eqs[1][3]*eqs[0][2];

                    /* Parametrise x and y in terms of some t. */
                    if (abs(eq[0]) < abs(eq[1])) {
                        /* Parameter is x. */
                        xt[0] = 1; xt[1] = 0;
                        yt[0] = -eq[0]/eq[1]; yt[1] = eq[2]/eq[1];
                    } else {
                        /* Parameter is y. */
                        yt[0] = 1; yt[1] = 0;
                        xt[0] = -eq[1]/eq[0]; xt[1] = eq[2]/eq[0];
                    }

                    /* Find a linear representation of r using eqs[0]. */
                    rt[0] = -(eqs[0][0]*xt[0] + eqs[0][1]*yt[0])/eqs[0][2];
                    rt[1] = (eqs[0][3] - eqs[0][0]*xt[1] -
                             eqs[0][1]*yt[1])/eqs[0][2];

                    /* Construct the quadratic equation. */
                    q[0] = -rt[0]*rt[0];
                    q[1] = -2*rt[0]*rt[1];
                    q[2] = -rt[1]*rt[1];
                    q[0] += xt[0]*xt[0];
                    q[1] += 2*xt[0]*(xt[1]-dots[0]->x);
                    q[2] += (xt[1]-dots[0]->x)*(xt[1]-dots[0]->x);
                    q[0] += yt[0]*yt[0];
                    q[1] += 2*yt[0]*(yt[1]-dots[0]->y);
                    q[2] += (yt[1]-dots[0]->y)*(yt[1]-dots[0]->y);

                    /* And solve it. */
                    disc = q[1]*q[1] - 4*q[0]*q[2];
                    if (disc >= 0) {
                        double t;

                        disc = sqrt(disc);

                        t = (-q[1] + disc) / (2*q[0]);
                        cx[cn] = xt[0]*t + xt[1];
                        cy[cn] = yt[0]*t + yt[1];
                        cn++;

                        t = (-q[1] - disc) / (2*q[0]);
                        cx[cn] = xt[0]*t + xt[1];
                        cy[cn] = yt[0]*t + yt[1];
                        cn++;
                    }
                } else if (nedges == 1) {
                    /*
                     * Two dots and an edge. This one's another
                     * quadratic equation.
                     *
                     * The point we want must lie on the perpendicular
                     * bisector of the two dots; that much is obvious.
                     * So we can construct a parametrisation of that
                     * bisecting line, giving linear formulae for x,y
                     * in terms of t. We can also express the distance
                     * from the edge as such a linear formula.
                     *
                     * Then we set that equal to the radius of the
                     * circle passing through the two points, which is
                     * a Pythagoras exercise; that gives rise to a
                     * quadratic in t, which we solve.
                     */
                    double xt[2], yt[2], rt[2]; /* a,b: {x,y,r}=at+b */
                    double q[3];                /* a,b,c: at^2+bt+c=0 */
                    double disc;
                    double halfsep;

                    /* Find parametric formulae for x,y. */
                    {
                        int x1 = dots[0]->x, x2 = dots[1]->x;
                        int y1 = dots[0]->y, y2 = dots[1]->y;
                        int dx = x2-x1, dy = y2-y1;
                        double d = sqrt((double)dx*dx + (double)dy*dy);

                        xt[1] = (x1+x2)/2.0;
                        yt[1] = (y1+y2)/2.0;
                        /* It's convenient if we have t at standard scale. */
                        xt[0] = -dy/d;
                        yt[0] = dx/d;

                        /* Also note down half the separation between
                         * the dots, for use in computing the circle radius. */
                        halfsep = 0.5*d;
                    }

                    /* Find a parametric formula for r. */
                    {
                        int x1 = edgedot1[0]->x, x2 = edgedot2[0]->x;
                        int y1 = edgedot1[0]->y, y2 = edgedot2[0]->y;
                        int dx = x2-x1, dy = y2-y1;
                        double d = sqrt((double)dx*dx + (double)dy*dy);
                        rt[0] = (xt[0]*dy - yt[0]*dx) / d;
                        rt[1] = ((xt[1]-x1)*dy - (yt[1]-y1)*dx) / d;
                    }

                    /* Construct the quadratic equation. */
                    q[0] = rt[0]*rt[0];
                    q[1] = 2*rt[0]*rt[1];
                    q[2] = rt[1]*rt[1];
                    q[0] -= 1;
                    q[2] -= halfsep*halfsep;

                    /* And solve it. */
                    disc = q[1]*q[1] - 4*q[0]*q[2];
                    if (disc >= 0) {
                        double t;

                        disc = sqrt(disc);

                        t = (-q[1] + disc) / (2*q[0]);
                        cx[cn] = xt[0]*t + xt[1];
                        cy[cn] = yt[0]*t + yt[1];
                        cn++;

                        t = (-q[1] - disc) / (2*q[0]);
                        cx[cn] = xt[0]*t + xt[1];
                        cy[cn] = yt[0]*t + yt[1];
                        cn++;
                    }
                } else if (nedges == 0) {
                    /*
                     * Three dots. This is another linear matrix
                     * equation, this time with each row of the matrix
                     * representing the perpendicular bisector between
                     * two of the points. Of course we only need two
                     * such lines to find their intersection, so we
                     * need only solve a 2x2 matrix equation.
                     */

                    double matrix[4], vector[2], vector2[2];
                    int m;

                    for (m = 0; m < 2; m++) {
                        int x1 = dots[m]->x, x2 = dots[m+1]->x;
                        int y1 = dots[m]->y, y2 = dots[m+1]->y;
                        int dx = x2-x1, dy = y2-y1;

                        /*
                         * ((x,y) - (x1,y1)) . (dx,dy) = 1/2 |(dx,dy)|^2
                         *
                         * => 2x dx + 2y dy = dx^2+dy^2 + (2 x1 dx + 2 y1 dy)
                         */
                        matrix[2*m+0] = 2*dx;
                        matrix[2*m+1] = 2*dy;
                        vector[m] = ((double)dx*dx + (double)dy*dy +
                                     2.0*x1*dx + 2.0*y1*dy);
                    }

                    if (solve_2x2_matrix(matrix, vector, vector2)) {
                        cx[cn] = vector2[0];
                        cy[cn] = vector2[1];
                        cn++;
                    }
                }

                /*
                 * Now go through our candidate points and see if any
                 * of them are better than what we've got so far.
                 */
                for (m = 0; m < cn; m++) {
                    double x = cx[m], y = cy[m];

                    /*
                     * First, disqualify the point if it's not inside
                     * the polygon, which we work out by counting the
                     * edges to the right of the point. (For
                     * tiebreaking purposes when edges start or end on
                     * our y-coordinate or go right through it, we
                     * consider our point to be offset by a small
                     * _positive_ epsilon in both the x- and
                     * y-direction.)
                     */
                    int e, in = 0;
                    for (e = 0; e < f->order; e++) {
                        int xs = f->edges[e]->dot1->x;
                        int xe = f->edges[e]->dot2->x;
                        int ys = f->edges[e]->dot1->y;
                        int ye = f->edges[e]->dot2->y;
                        if ((y >= ys && y < ye) || (y >= ye && y < ys)) {
                            /*
                             * The line goes past our y-position. Now we need
                             * to know if its x-coordinate when it does so is
                             * to our right.
                             *
                             * The x-coordinate in question is mathematically
                             * (y - ys) * (xe - xs) / (ye - ys), and we want
                             * to know whether (x - xs) >= that. Of course we
                             * avoid the division, so we can work in integers;
                             * to do this we must multiply both sides of the
                             * inequality by ye - ys, which means we must
                             * first check that's not negative.
                             */
                            int num = xe - xs, denom = ye - ys;
                            if (denom < 0) {
                                num = -num;
                                denom = -denom;
                            }
                            if ((x - xs) * denom >= (y - ys) * num)
                                in ^= 1;
                        }
                    }

                    if (in) {
#ifdef HUGE_VAL
                        double mindist = HUGE_VAL;
#else
#ifdef DBL_MAX
                        double mindist = DBL_MAX;
#else
#error No way to get maximum floating-point number.
#endif
#endif
                        int e, d;

                        /*
                         * This point is inside the polygon, so now we check
                         * its minimum distance to every edge and corner.
                         * First the corners ...
                         */
                        for (d = 0; d < f->order; d++) {
                            int xp = f->dots[d]->x;
                            int yp = f->dots[d]->y;
                            double dx = x - xp, dy = y - yp;
                            double dist = dx*dx + dy*dy;
                            if (mindist > dist)
                                mindist = dist;
                        }

                        /*
                         * ... and now also check the perpendicular distance
                         * to every edge, if the perpendicular lies between
                         * the edge's endpoints.
                         */
                        for (e = 0; e < f->order; e++) {
                            int xs = f->edges[e]->dot1->x;
                            int xe = f->edges[e]->dot2->x;
                            int ys = f->edges[e]->dot1->y;
                            int ye = f->edges[e]->dot2->y;

                            /*
                             * If s and e are our endpoints, and p our
                             * candidate circle centre, the foot of a
                             * perpendicular from p to the line se lies
                             * between s and e if and only if (p-s).(e-s) lies
                             * strictly between 0 and (e-s).(e-s).
                             */
                            int edx = xe - xs, edy = ye - ys;
                            double pdx = x - xs, pdy = y - ys;
                            double pde = pdx * edx + pdy * edy;
                            long ede = (long)edx * edx + (long)edy * edy;
                            if (0 < pde && pde < ede) {
                                /*
                                 * Yes, the nearest point on this edge is
                                 * closer than either endpoint, so we must
                                 * take it into account by measuring the
                                 * perpendicular distance to the edge and
                                 * checking its square against mindist.
                                 */

                                double pdre = pdx * edy - pdy * edx;
                                double sqlen = pdre * pdre / ede;

                                if (mindist > sqlen)
                                    mindist = sqlen;
                            }
                        }

                        /*
                         * Right. Now we know the biggest circle around this
                         * point, so we can check it against bestdist.
                         */
                        if (bestdist < mindist) {
                            bestdist = mindist;
                            xbest = x;
                            ybest = y;
                        }
                    }
                }

                if (k < f->order)
                    nedges--;
                else
                    ndots--;
            }
            if (j < f->order)
                nedges--;
            else
                ndots--;
        }
        if (i < f->order)
            nedges--;
        else
            ndots--;
    }

    assert(bestdist > 0);

    f->has_incentre = TRUE;
    f->ix = xbest + 0.5;               /* round to nearest */
    f->iy = ybest + 0.5;
}