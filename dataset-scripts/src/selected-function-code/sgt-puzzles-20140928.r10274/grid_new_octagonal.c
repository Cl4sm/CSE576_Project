static grid *grid_new_octagonal(int width, int height, const char *desc)
{
    int x, y;
    int a = OCTAGONAL_A;
    int b = OCTAGONAL_B;

    /* Upper bounds - don't have to be exact */
    int max_faces = 2 * width * height;
    int max_dots = 4 * (width + 1) * (height + 1);

    tree234 *points;

    grid *g = grid_empty();
    g->tilesize = OCTAGONAL_TILESIZE;
    g->faces = snewn(max_faces, grid_face);
    g->dots = snewn(max_dots, grid_dot);

    points = newtree234(grid_point_cmp_fn);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            grid_dot *d;
            /* cell position */
            int px = (2*a + b) * x;
            int py = (2*a + b) * y;
            /* octagon */
            grid_face_add_new(g, 8);
            d = grid_get_dot(g, points, px + a, py);
            grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, px + a + b, py);
            grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, px + 2*a + b, py + a);
            grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, px + 2*a + b, py + a + b);
            grid_face_set_dot(g, d, 3);
            d = grid_get_dot(g, points, px + a + b, py + 2*a + b);
            grid_face_set_dot(g, d, 4);
            d = grid_get_dot(g, points, px + a, py + 2*a + b);
            grid_face_set_dot(g, d, 5);
            d = grid_get_dot(g, points, px, py + a + b);
            grid_face_set_dot(g, d, 6);
            d = grid_get_dot(g, points, px, py + a);
            grid_face_set_dot(g, d, 7);

            /* diamond */
            if ((x > 0) && (y > 0)) {
                grid_face_add_new(g, 4);
                d = grid_get_dot(g, points, px, py - a);
                grid_face_set_dot(g, d, 0);
                d = grid_get_dot(g, points, px + a, py);
                grid_face_set_dot(g, d, 1);
                d = grid_get_dot(g, points, px, py + a);
                grid_face_set_dot(g, d, 2);
                d = grid_get_dot(g, points, px - a, py);
                grid_face_set_dot(g, d, 3);
            }
        }
    }

    freetree234(points);
    assert(g->num_faces <= max_faces);
    assert(g->num_dots <= max_dots);

    grid_make_consistent(g);
    return g;
}