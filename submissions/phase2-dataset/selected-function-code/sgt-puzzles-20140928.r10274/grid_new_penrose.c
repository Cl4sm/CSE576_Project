static grid *grid_new_penrose(int width, int height, int which, const char *desc)
{
    int max_faces, max_dots, tilesize = PENROSE_TILESIZE;
    int xsz, ysz, xoff, yoff, aoff;
    double rradius;

    tree234 *points;
    grid *g;

    penrose_state ps;
    setface_ctx sf_ctx;

    penrose_calculate_size(which, tilesize, width, height,
                           &rradius, &ps.start_size, &ps.max_depth);

    debug(("penrose: w%d h%d, tile size %d, start size %d, depth %d",
           width, height, tilesize, ps.start_size, ps.max_depth));

    ps.new_tile = set_faces;
    ps.ctx = &sf_ctx;

    max_faces = (width*3) * (height*3); /* somewhat paranoid... */
    max_dots = max_faces * 4; /* ditto... */

    g = grid_empty();
    g->tilesize = tilesize;
    g->faces = snewn(max_faces, grid_face);
    g->dots = snewn(max_dots, grid_dot);

    points = newtree234(grid_point_cmp_fn);

    memset(&sf_ctx, 0, sizeof(sf_ctx));
    sf_ctx.g = g;
    sf_ctx.points = points;

    if (desc != NULL) {
        if (sscanf(desc, "G%d,%d,%d", &xoff, &yoff, &aoff) != 3)
            assert(!"Invalid grid description.");
    } else {
        xoff = yoff = aoff = 0;
    }

    xsz = width * tilesize;
    ysz = height * tilesize;

    sf_ctx.xmin = xoff - xsz/2;
    sf_ctx.xmax = xoff + xsz/2;
    sf_ctx.ymin = yoff - ysz/2;
    sf_ctx.ymax = yoff + ysz/2;

    debug(("penrose: centre (%f, %f) xsz %f ysz %f",
           0.0, 0.0, xsz, ysz));
    debug(("penrose: x range (%f --> %f), y range (%f --> %f)",
           sf_ctx.xmin, sf_ctx.xmax, sf_ctx.ymin, sf_ctx.ymax));

    penrose(&ps, which, aoff);

    freetree234(points);
    assert(g->num_faces <= max_faces);
    assert(g->num_dots <= max_dots);

    debug(("penrose: %d faces total (equivalent to %d wide by %d high)",
           g->num_faces, g->num_faces/height, g->num_faces/width));

    /*
     * Return NULL if we ended up with an empty grid, either because
     * the initial generation was over too small a rectangle to
     * encompass any face or because grid_trim_vigorously ended up
     * removing absolutely everything.
     */
    if (g->num_faces == 0 || g->num_dots == 0) {
        grid_free(g);
        return NULL;
    }
    grid_trim_vigorously(g);
    if (g->num_faces == 0 || g->num_dots == 0) {
        grid_free(g);
        return NULL;
    }

    grid_make_consistent(g);

    /*
     * Centre the grid in its originally promised rectangle.
     */
    g->lowest_x -= ((sf_ctx.xmax - sf_ctx.xmin) -
                    (g->highest_x - g->lowest_x)) / 2;
    g->highest_x = g->lowest_x + (sf_ctx.xmax - sf_ctx.xmin);
    g->lowest_y -= ((sf_ctx.ymax - sf_ctx.ymin) -
                    (g->highest_y - g->lowest_y)) / 2;
    g->highest_y = g->lowest_y + (sf_ctx.ymax - sf_ctx.ymin);

    return g;
}