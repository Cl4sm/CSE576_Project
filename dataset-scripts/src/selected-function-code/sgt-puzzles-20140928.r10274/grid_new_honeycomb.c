static grid *grid_new_honeycomb(int width, int height, const char *desc)
{
    int x, y;
    int a = HONEY_A;
    int b = HONEY_B;

    /* Upper bounds - don't have to be exact */
    int max_faces = width * height;
    int max_dots = 2 * (width + 1) * (height + 1);

    tree234 *points;

    grid *g = grid_empty();
    g->tilesize = HONEY_TILESIZE;
    g->faces = snewn(max_faces, grid_face);
    g->dots = snewn(max_dots, grid_dot);

    points = newtree234(grid_point_cmp_fn);

    /* generate hexagonal faces */
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            grid_dot *d;
            /* face centre */
            int cx = 3 * a * x;
            int cy = 2 * b * y;
            if (x % 2)
                cy += b;
            grid_face_add_new(g, 6);

            d = grid_get_dot(g, points, cx - a, cy - b);
            grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, cx + a, cy - b);
            grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, cx + 2*a, cy);
            grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, cx + a, cy + b);
            grid_face_set_dot(g, d, 3);
            d = grid_get_dot(g, points, cx - a, cy + b);
            grid_face_set_dot(g, d, 4);
            d = grid_get_dot(g, points, cx - 2*a, cy);
            grid_face_set_dot(g, d, 5);
        }
    }

    freetree234(points);
    assert(g->num_faces <= max_faces);
    assert(g->num_dots <= max_dots);

    grid_make_consistent(g);
    return g;
}