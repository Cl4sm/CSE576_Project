static grid *grid_new_greathexagonal(int width, int height, const char *desc)
{
    int x, y;
    int a = GREATHEX_A;
    int b = GREATHEX_B;

    /* Upper bounds - don't have to be exact */
    int max_faces = 6 * (width + 1) * (height + 1);
    int max_dots = 6 * width * height;

    tree234 *points;

    grid *g = grid_empty();
    g->tilesize = GREATHEX_TILESIZE;
    g->faces = snewn(max_faces, grid_face);
    g->dots = snewn(max_dots, grid_dot);

    points = newtree234(grid_point_cmp_fn);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            grid_dot *d;
            /* centre of hexagon */
            int px = (3*a + b) * x;
            int py = (2*a + 2*b) * y;
            if (x % 2)
                py += a + b;

            /* hexagon */
            grid_face_add_new(g, 6);
            d = grid_get_dot(g, points, px - a, py - b);
            grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, px + a, py - b);
            grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, px + 2*a, py);
            grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, px + a, py + b);
            grid_face_set_dot(g, d, 3);
            d = grid_get_dot(g, points, px - a, py + b);
            grid_face_set_dot(g, d, 4);
            d = grid_get_dot(g, points, px - 2*a, py);
            grid_face_set_dot(g, d, 5);

            /* square below hexagon */
            if (y < height - 1) {
                grid_face_add_new(g, 4);
                d = grid_get_dot(g, points, px - a, py + b);
                grid_face_set_dot(g, d, 0);
                d = grid_get_dot(g, points, px + a, py + b);
                grid_face_set_dot(g, d, 1);
                d = grid_get_dot(g, points, px + a, py + 2*a + b);
                grid_face_set_dot(g, d, 2);
                d = grid_get_dot(g, points, px - a, py + 2*a + b);
                grid_face_set_dot(g, d, 3);
            }

            /* square below right */
            if ((x < width - 1) && (((x % 2) == 0) || (y < height - 1))) {
                grid_face_add_new(g, 4);
                d = grid_get_dot(g, points, px + 2*a, py);
                grid_face_set_dot(g, d, 0);
                d = grid_get_dot(g, points, px + 2*a + b, py + a);
                grid_face_set_dot(g, d, 1);
                d = grid_get_dot(g, points, px + a + b, py + a + b);
                grid_face_set_dot(g, d, 2);
                d = grid_get_dot(g, points, px + a, py + b);
                grid_face_set_dot(g, d, 3);
            }

            /* square below left */
            if ((x > 0) && (((x % 2) == 0) || (y < height - 1))) {
                grid_face_add_new(g, 4);
                d = grid_get_dot(g, points, px - 2*a, py);
                grid_face_set_dot(g, d, 0);
                d = grid_get_dot(g, points, px - a, py + b);
                grid_face_set_dot(g, d, 1);
                d = grid_get_dot(g, points, px - a - b, py + a + b);
                grid_face_set_dot(g, d, 2);
                d = grid_get_dot(g, points, px - 2*a - b, py + a);
                grid_face_set_dot(g, d, 3);
            }
           
            /* Triangle below right */
            if ((x < width - 1) && (y < height - 1)) {
                grid_face_add_new(g, 3);
                d = grid_get_dot(g, points, px + a, py + b);
                grid_face_set_dot(g, d, 0);
                d = grid_get_dot(g, points, px + a + b, py + a + b);
                grid_face_set_dot(g, d, 1);
                d = grid_get_dot(g, points, px + a, py + 2*a + b);
                grid_face_set_dot(g, d, 2);
            }

            /* Triangle below left */
            if ((x > 0) && (y < height - 1)) {
                grid_face_add_new(g, 3);
                d = grid_get_dot(g, points, px - a, py + b);
                grid_face_set_dot(g, d, 0);
                d = grid_get_dot(g, points, px - a, py + 2*a + b);
                grid_face_set_dot(g, d, 1);
                d = grid_get_dot(g, points, px - a - b, py + a + b);
                grid_face_set_dot(g, d, 2);
            }
        }
    }

    freetree234(points);
    assert(g->num_faces <= max_faces);
    assert(g->num_dots <= max_dots);

    grid_make_consistent(g);
    return g;
}