static grid *grid_new_square(int width, int height, const char *desc)
{
    int x, y;
    /* Side length */
    int a = SQUARE_TILESIZE;

    /* Upper bounds - don't have to be exact */
    int max_faces = width * height;
    int max_dots = (width + 1) * (height + 1);

    tree234 *points;

    grid *g = grid_empty();
    g->tilesize = a;
    g->faces = snewn(max_faces, grid_face);
    g->dots = snewn(max_dots, grid_dot);

    points = newtree234(grid_point_cmp_fn);

    /* generate square faces */
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            grid_dot *d;
            /* face position */
            int px = a * x;
            int py = a * y;

            grid_face_add_new(g, 4);
            d = grid_get_dot(g, points, px, py);
            grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, px + a, py);
            grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, px + a, py + a);
            grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, px, py + a);
            grid_face_set_dot(g, d, 3);
        }
    }

    freetree234(points);
    assert(g->num_faces <= max_faces);
    assert(g->num_dots <= max_dots);

    grid_make_consistent(g);
    return g;
}