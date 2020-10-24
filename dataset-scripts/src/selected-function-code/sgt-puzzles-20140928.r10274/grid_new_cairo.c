static grid *grid_new_cairo(int width, int height, const char *desc)
{
    int x, y;
    int a = CAIRO_A;
    int b = CAIRO_B;

    /* Upper bounds - don't have to be exact */
    int max_faces = 2 * width * height;
    int max_dots = 3 * (width + 1) * (height + 1);

    tree234 *points;

    grid *g = grid_empty();
    g->tilesize = CAIRO_TILESIZE;
    g->faces = snewn(max_faces, grid_face);
    g->dots = snewn(max_dots, grid_dot);

    points = newtree234(grid_point_cmp_fn);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            grid_dot *d;
            /* cell position */
            int px = 2 * b * x;
            int py = 2 * b * y;

            /* horizontal pentagons */
            if (y > 0) {
                grid_face_add_new(g, 5);
                if ((x + y) % 2) {
                    d = grid_get_dot(g, points, px + a, py - b);
                    grid_face_set_dot(g, d, 0);
                    d = grid_get_dot(g, points, px + 2*b - a, py - b);
                    grid_face_set_dot(g, d, 1);
                    d = grid_get_dot(g, points, px + 2*b, py);
                    grid_face_set_dot(g, d, 2);
                    d = grid_get_dot(g, points, px + b, py + a);
                    grid_face_set_dot(g, d, 3);
                    d = grid_get_dot(g, points, px, py);
                    grid_face_set_dot(g, d, 4);
                } else {
                    d = grid_get_dot(g, points, px, py);
                    grid_face_set_dot(g, d, 0);
                    d = grid_get_dot(g, points, px + b, py - a);
                    grid_face_set_dot(g, d, 1);
                    d = grid_get_dot(g, points, px + 2*b, py);
                    grid_face_set_dot(g, d, 2);
                    d = grid_get_dot(g, points, px + 2*b - a, py + b);
                    grid_face_set_dot(g, d, 3);
                    d = grid_get_dot(g, points, px + a, py + b);
                    grid_face_set_dot(g, d, 4);
                }
            }
            /* vertical pentagons */
            if (x > 0) {
                grid_face_add_new(g, 5);
                if ((x + y) % 2) {
                    d = grid_get_dot(g, points, px, py);
                    grid_face_set_dot(g, d, 0);
                    d = grid_get_dot(g, points, px + b, py + a);
                    grid_face_set_dot(g, d, 1);
                    d = grid_get_dot(g, points, px + b, py + 2*b - a);
                    grid_face_set_dot(g, d, 2);
                    d = grid_get_dot(g, points, px, py + 2*b);
                    grid_face_set_dot(g, d, 3);
                    d = grid_get_dot(g, points, px - a, py + b);
                    grid_face_set_dot(g, d, 4);
                } else {
                    d = grid_get_dot(g, points, px, py);
                    grid_face_set_dot(g, d, 0);
                    d = grid_get_dot(g, points, px + a, py + b);
                    grid_face_set_dot(g, d, 1);
                    d = grid_get_dot(g, points, px, py + 2*b);
                    grid_face_set_dot(g, d, 2);
                    d = grid_get_dot(g, points, px - b, py + 2*b - a);
                    grid_face_set_dot(g, d, 3);
                    d = grid_get_dot(g, points, px - b, py + a);
                    grid_face_set_dot(g, d, 4);
                }
            }
        }
    }

    freetree234(points);
    assert(g->num_faces <= max_faces);
    assert(g->num_dots <= max_dots);

    grid_make_consistent(g);
    return g;
}