static grid *grid_new_triangular(int width, int height, const char *desc)
{
    int x,y;
    int version = (desc == NULL ? -1 : atoi(desc));
    
    /* Vector for side of triangle - ratio is close to sqrt(3) */
    int vec_x = TRIANGLE_VEC_X;
    int vec_y = TRIANGLE_VEC_Y;
    
    int index;

    /* convenient alias */
    int w = width + 1;

    grid *g = grid_empty();
    g->tilesize = TRIANGLE_TILESIZE;

    if (version == -1) {
        /*
         * Old-style triangular grid generation, preserved as-is for
         * backwards compatibility with old game ids, in which it's
         * just a little asymmetric and there are 'ears' (faces linked
         * to only one other face) at two grid corners.
         *
         * Example old-style game ids, which should still work, and in
         * which you should see the ears in the TL/BR corners on the
         * first one and in the TL/BL corners on the second:
         *
         *   5x5t1:2c2a1a2a201a1a1a1112a1a2b1211f0b21a2a2a0a
         *   5x6t1:a022a212h1a1d1a12c2b11a012b1a20d1a0a12e
         */

        g->num_faces = width * height * 2;
        g->num_dots = (width + 1) * (height + 1);
        g->faces = snewn(g->num_faces, grid_face);
        g->dots = snewn(g->num_dots, grid_dot);

        /* generate dots */
        index = 0;
        for (y = 0; y <= height; y++) {
            for (x = 0; x <= width; x++) {
                grid_dot *d = g->dots + index;
                /* odd rows are offset to the right */
                d->order = 0;
                d->edges = NULL;
                d->faces = NULL;
                d->x = x * 2 * vec_x + ((y % 2) ? vec_x : 0);
                d->y = y * vec_y;
                index++;
            }
        }
    
        /* generate faces */
        index = 0;
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                /* initialise two faces for this (x,y) */
                grid_face *f1 = g->faces + index;
                grid_face *f2 = f1 + 1;
                f1->edges = NULL;
                f1->order = 3;
                f1->dots = snewn(f1->order, grid_dot*);
                f1->has_incentre = FALSE;
                f2->edges = NULL;
                f2->order = 3;
                f2->dots = snewn(f2->order, grid_dot*);
                f2->has_incentre = FALSE;

                /* face descriptions depend on whether the row-number is
                 * odd or even */
                if (y % 2) {
                    f1->dots[0] = g->dots + y       * w + x;
                    f1->dots[1] = g->dots + (y + 1) * w + x + 1;
                    f1->dots[2] = g->dots + (y + 1) * w + x;
                    f2->dots[0] = g->dots + y       * w + x;
                    f2->dots[1] = g->dots + y       * w + x + 1;
                    f2->dots[2] = g->dots + (y + 1) * w + x + 1;
                } else {
                    f1->dots[0] = g->dots + y       * w + x;
                    f1->dots[1] = g->dots + y       * w + x + 1;
                    f1->dots[2] = g->dots + (y + 1) * w + x;
                    f2->dots[0] = g->dots + y       * w + x + 1;
                    f2->dots[1] = g->dots + (y + 1) * w + x + 1;
                    f2->dots[2] = g->dots + (y + 1) * w + x;
                }
                index += 2;
            }
        }
    } else {
        /*
         * New-style approach, in which there are never any 'ears',
         * and if height is even then the grid is nicely 4-way
         * symmetric.
         *
         * Example new-style grids:
         *
         *   5x5t1:0_21120b11a1a01a1a00c1a0b211021c1h1a2a1a0a
         *   5x6t1:0_a1212c22c2a02a2f22a0c12a110d0e1c0c0a101121a1
         */
        tree234 *points = newtree234(grid_point_cmp_fn);
        /* Upper bounds - don't have to be exact */
        int max_faces = height * (2*width+1);
        int max_dots = (height+1) * (width+1) * 4;

        g->faces = snewn(max_faces, grid_face);
        g->dots = snewn(max_dots, grid_dot);

        for (y = 0; y < height; y++) {
            /*
             * Each row contains (width+1) triangles one way up, and
             * (width) triangles the other way up. Which way up is
             * which varies with parity of y. Also, the dots around
             * each face will flip direction with parity of y, so we
             * set up n1 and n2 to cope with that easily.
             */
            int y0, y1, n1, n2;
            y0 = y1 = y * vec_y;
            if (y % 2) {
                y1 += vec_y;
                n1 = 2; n2 = 1;
            } else {
                y0 += vec_y;
                n1 = 1; n2 = 2;
            }

            for (x = 0; x <= width; x++) {
                int x0 = 2*x * vec_x, x1 = x0 + vec_x, x2 = x1 + vec_x;

                /*
                 * If the grid has odd height, then we skip the first
                 * and last triangles on this row, otherwise they'll
                 * end up as ears.
                 */
                if (height % 2 == 1 && y == height-1 && (x == 0 || x == width))
                    continue;

                grid_face_add_new(g, 3);
                grid_face_set_dot(g, grid_get_dot(g, points, x0, y0), 0);
                grid_face_set_dot(g, grid_get_dot(g, points, x1, y1), n1);
                grid_face_set_dot(g, grid_get_dot(g, points, x2, y0), n2);
            }

            for (x = 0; x < width; x++) {
                int x0 = (2*x+1) * vec_x, x1 = x0 + vec_x, x2 = x1 + vec_x;

                grid_face_add_new(g, 3);
                grid_face_set_dot(g, grid_get_dot(g, points, x0, y1), 0);
                grid_face_set_dot(g, grid_get_dot(g, points, x1, y0), n2);
                grid_face_set_dot(g, grid_get_dot(g, points, x2, y1), n1);
            }
        }

        freetree234(points);
        assert(g->num_faces <= max_faces);
        assert(g->num_dots <= max_dots);
    }

    grid_make_consistent(g);
    return g;
}