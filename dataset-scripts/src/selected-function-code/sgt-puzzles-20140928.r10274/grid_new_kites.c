static grid *grid_new_kites(int width, int height, const char *desc)
{
    int x, y;
    int a = KITE_A;
    int b = KITE_B;

    /* Upper bounds - don't have to be exact */
    int max_faces = 6 * width * height;
    int max_dots = 6 * (width + 1) * (height + 1);

    tree234 *points;

    grid *g = grid_empty();
    g->tilesize = KITE_TILESIZE;
    g->faces = snewn(max_faces, grid_face);
    g->dots = snewn(max_dots, grid_dot);

    points = newtree234(grid_point_cmp_fn);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            grid_dot *d;
            /* position of order-6 dot */
            int px = 4*b * x;
            int py = 6*a * y;
            if (y % 2)
                px += 2*b;

            /* kite pointing up-left */
            grid_face_add_new(g, 4);
            d = grid_get_dot(g, points, px, py);
            grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, px + 2*b, py);
            grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, px + 2*b, py + 2*a);
            grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, px + b, py + 3*a);
            grid_face_set_dot(g, d, 3);

            /* kite pointing up */
            grid_face_add_new(g, 4);
            d = grid_get_dot(g, points, px, py);
            grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, px + b, py + 3*a);
            grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, px, py + 4*a);
            grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, px - b, py + 3*a);
            grid_face_set_dot(g, d, 3);

            /* kite pointing up-right */
            grid_face_add_new(g, 4);
            d = grid_get_dot(g, points, px, py);
            grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, px - b, py + 3*a);
            grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, px - 2*b, py + 2*a);
            grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, px - 2*b, py);
            grid_face_set_dot(g, d, 3);

            /* kite pointing down-right */
            grid_face_add_new(g, 4);
            d = grid_get_dot(g, points, px, py);
            grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, px - 2*b, py);
            grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, px - 2*b, py - 2*a);
            grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, px - b, py - 3*a);
            grid_face_set_dot(g, d, 3);

            /* kite pointing down */
            grid_face_add_new(g, 4);
            d = grid_get_dot(g, points, px, py);
            grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, px - b, py - 3*a);
            grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, px, py - 4*a);
            grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, px + b, py - 3*a);
            grid_face_set_dot(g, d, 3);

            /* kite pointing down-left */
            grid_face_add_new(g, 4);
            d = grid_get_dot(g, points, px, py);
            grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, px + b, py - 3*a);
            grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, px + 2*b, py - 2*a);
            grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, px + 2*b, py);
            grid_face_set_dot(g, d, 3);
        }
    }

    freetree234(points);
    assert(g->num_faces <= max_faces);
    assert(g->num_dots <= max_dots);

    grid_make_consistent(g);
    return g;
}