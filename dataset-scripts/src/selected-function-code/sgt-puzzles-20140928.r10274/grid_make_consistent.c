static void grid_make_consistent(grid *g)
{
    int i;
    tree234 *incomplete_edges;
    grid_edge *next_new_edge; /* Where new edge will go into g->edges */

    grid_debug_basic(g);

    /* ====== Stage 1 ======
     * Generate edges
     */

    /* We know how many dots and faces there are, so we can find the exact
     * number of edges from Euler's polyhedral formula: F + V = E + 2 .
     * We use "-1", not "-2" here, because Euler's formula includes the
     * infinite face, which we don't count. */
    g->num_edges = g->num_faces + g->num_dots - 1;
    g->edges = snewn(g->num_edges, grid_edge);
    next_new_edge = g->edges;

    /* Iterate over faces, and over each face's dots, generating edges as we
     * go.  As we find each new edge, we can immediately fill in the edge's
     * dots, but only one of the edge's faces.  Later on in the iteration, we
     * will find the same edge again (unless it's on the border), but we will
     * know the other face.
     * For efficiency, maintain a list of the incomplete edges, sorted by
     * their dots. */
    incomplete_edges = newtree234(grid_edge_bydots_cmpfn);
    for (i = 0; i < g->num_faces; i++) {
        grid_face *f = g->faces + i;
        int j;
        for (j = 0; j < f->order; j++) {
            grid_edge e; /* fake edge for searching */
            grid_edge *edge_found;
            int j2 = j + 1;
            if (j2 == f->order)
                j2 = 0;
            e.dot1 = f->dots[j];
            e.dot2 = f->dots[j2];
            /* Use del234 instead of find234, because we always want to
             * remove the edge if found */
            edge_found = del234(incomplete_edges, &e);
            if (edge_found) {
                /* This edge already added, so fill out missing face.
                 * Edge is already removed from incomplete_edges. */
                edge_found->face2 = f;
            } else {
                assert(next_new_edge - g->edges < g->num_edges);
                next_new_edge->dot1 = e.dot1;
                next_new_edge->dot2 = e.dot2;
                next_new_edge->face1 = f;
                next_new_edge->face2 = NULL; /* potentially infinite face */
                add234(incomplete_edges, next_new_edge);
                ++next_new_edge;
            }
        }
    }
    freetree234(incomplete_edges);
    
    /* ====== Stage 2 ======
     * For each face, build its edge list.
     */

    /* Allocate space for each edge list.  Can do this, because each face's
     * edge-list is the same size as its dot-list. */
    for (i = 0; i < g->num_faces; i++) {
        grid_face *f = g->faces + i;
        int j;
        f->edges = snewn(f->order, grid_edge*);
        /* Preload with NULLs, to help detect potential bugs. */
        for (j = 0; j < f->order; j++)
            f->edges[j] = NULL;
    }
    
    /* Iterate over each edge, and over both its faces.  Add this edge to
     * the face's edge-list, after finding where it should go in the
     * sequence. */
    for (i = 0; i < g->num_edges; i++) {
        grid_edge *e = g->edges + i;
        int j;
        for (j = 0; j < 2; j++) {
            grid_face *f = j ? e->face2 : e->face1;
            int k, k2;
            if (f == NULL) continue;
            /* Find one of the dots around the face */
            for (k = 0; k < f->order; k++) {
                if (f->dots[k] == e->dot1)
                    break; /* found dot1 */
            }
            assert(k != f->order); /* Must find the dot around this face */

            /* Labelling scheme: as we walk clockwise around the face,
             * starting at dot0 (f->dots[0]), we hit:
             * (dot0), edge0, dot1, edge1, dot2,...
             *
             *     0
             *  0-----1
             *        |
             *        |1
             *        |
             *  3-----2
             *     2
             *
             * Therefore, edgeK joins dotK and dot{K+1}
             */
            
            /* Around this face, either the next dot or the previous dot
             * must be e->dot2.  Otherwise the edge is wrong. */
            k2 = k + 1;
            if (k2 == f->order)
                k2 = 0;
            if (f->dots[k2] == e->dot2) {
                /* dot1(k) and dot2(k2) go clockwise around this face, so add
                 * this edge at position k (see diagram). */
                assert(f->edges[k] == NULL);
                f->edges[k] = e;
                continue;
            }
            /* Try previous dot */
            k2 = k - 1;
            if (k2 == -1)
                k2 = f->order - 1;
            if (f->dots[k2] == e->dot2) {
                /* dot1(k) and dot2(k2) go anticlockwise around this face. */
                assert(f->edges[k2] == NULL);
                f->edges[k2] = e;
                continue;
            }
            assert(!"Grid broken: bad edge-face relationship");
        }
    }

    /* ====== Stage 3 ======
     * For each dot, build its edge-list and face-list.
     */

    /* We don't know how many edges/faces go around each dot, so we can't
     * allocate the right space for these lists.  Pre-compute the sizes by
     * iterating over each edge and recording a tally against each dot. */
    for (i = 0; i < g->num_dots; i++) {
        g->dots[i].order = 0;
    }
    for (i = 0; i < g->num_edges; i++) {
        grid_edge *e = g->edges + i;
        ++(e->dot1->order);
        ++(e->dot2->order);
    }
    /* Now we have the sizes, pre-allocate the edge and face lists. */
    for (i = 0; i < g->num_dots; i++) {
        grid_dot *d = g->dots + i;
        int j;
        assert(d->order >= 2); /* sanity check */
        d->edges = snewn(d->order, grid_edge*);
        d->faces = snewn(d->order, grid_face*);
        for (j = 0; j < d->order; j++) {
            d->edges[j] = NULL;
            d->faces[j] = NULL;
        }
    }
    /* For each dot, need to find a face that touches it, so we can seed
     * the edge-face-edge-face process around each dot. */
    for (i = 0; i < g->num_faces; i++) {
        grid_face *f = g->faces + i;
        int j;
        for (j = 0; j < f->order; j++) {
            grid_dot *d = f->dots[j];
            d->faces[0] = f;
        }
    }
    /* Each dot now has a face in its first slot.  Generate the remaining
     * faces and edges around the dot, by searching both clockwise and
     * anticlockwise from the first face.  Need to do both directions,
     * because of the possibility of hitting the infinite face, which
     * blocks progress.  But there's only one such face, so we will
     * succeed in finding every edge and face this way. */
    for (i = 0; i < g->num_dots; i++) {
        grid_dot *d = g->dots + i;
        int current_face1 = 0; /* ascends clockwise */
        int current_face2 = 0; /* descends anticlockwise */
        
        /* Labelling scheme: as we walk clockwise around the dot, starting
         * at face0 (d->faces[0]), we hit:
         * (face0), edge0, face1, edge1, face2,...
         *
         *       0
         *       |
         *    0  |  1
         *       |
         *  -----d-----1
         *       |
         *       |  2
         *       |
         *       2
         *
         * So, for example, face1 should be joined to edge0 and edge1,
         * and those edges should appear in an anticlockwise sense around
         * that face (see diagram). */
 
        /* clockwise search */
        while (TRUE) {
            grid_face *f = d->faces[current_face1];
            grid_edge *e;
            int j;
            assert(f != NULL);
            /* find dot around this face */
            for (j = 0; j < f->order; j++) {
                if (f->dots[j] == d)
                    break;
            }
            assert(j != f->order); /* must find dot */
            
            /* Around f, required edge is anticlockwise from the dot.  See
             * the other labelling scheme higher up, for why we subtract 1
             * from j. */
            j--;
            if (j == -1)
                j = f->order - 1;
            e = f->edges[j];
            d->edges[current_face1] = e; /* set edge */
            current_face1++;
            if (current_face1 == d->order)
                break;
            else {
                /* set face */
                d->faces[current_face1] =
                    (e->face1 == f) ? e->face2 : e->face1;
                if (d->faces[current_face1] == NULL)
                    break; /* cannot progress beyond infinite face */
            }
        }
        /* If the clockwise search made it all the way round, don't need to
         * bother with the anticlockwise search. */
        if (current_face1 == d->order)
            continue; /* this dot is complete, move on to next dot */
        
        /* anticlockwise search */
        while (TRUE) {
            grid_face *f = d->faces[current_face2];
            grid_edge *e;
            int j;
            assert(f != NULL);
            /* find dot around this face */
            for (j = 0; j < f->order; j++) {
                if (f->dots[j] == d)
                    break;
            }
            assert(j != f->order); /* must find dot */
            
            /* Around f, required edge is clockwise from the dot. */
            e = f->edges[j];
            
            current_face2--;
            if (current_face2 == -1)
                current_face2 = d->order - 1;
            d->edges[current_face2] = e; /* set edge */

            /* set face */
            if (current_face2 == current_face1)
                break;
            d->faces[current_face2] =
                    (e->face1 == f) ? e->face2 : e->face1;
            /* There's only 1 infinite face, so we must get all the way
             * to current_face1 before we hit it. */
            assert(d->faces[current_face2]);
        }
    }

    /* ====== Stage 4 ======
     * Compute other grid settings
     */

    /* Bounding rectangle */
    for (i = 0; i < g->num_dots; i++) {
        grid_dot *d = g->dots + i;
        if (i == 0) {
            g->lowest_x = g->highest_x = d->x;
            g->lowest_y = g->highest_y = d->y;
        } else {
            g->lowest_x = min(g->lowest_x, d->x);
            g->highest_x = max(g->highest_x, d->x);
            g->lowest_y = min(g->lowest_y, d->y);
            g->highest_y = max(g->highest_y, d->y);
        }
    }

    grid_debug_derived(g);
}