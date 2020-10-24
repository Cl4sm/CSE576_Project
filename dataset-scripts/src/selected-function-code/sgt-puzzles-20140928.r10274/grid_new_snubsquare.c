static grid *grid_new_snubsquare(int width, int height, const char *desc)
{
    int x, y;
    int a = SNUBSQUARE_A;
    int b = SNUBSQUARE_B;

    /* Upper bounds - don't have to be exact */
    int max_faces = 3 * width * height;
    int max_dots = 2 * (width + 1) * (height + 1);

    tree234 *points;

    grid *g = grid_empty();
    g->tilesize = SNUBSQUARE_TILESIZE;
    g->faces = snewn(max_faces, grid_face);
    g->dots = snewn(max_dots, grid_dot);

    points = newtree234(grid_point_cmp_fn);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            grid_dot *d;
            /* face position */
            int px = (a + b) * x;
            int py = (a + b) * y;

            /* generate square faces */
            grid_face_add_new(g, 4);
            if ((x + y) % 2) {
                d = grid_get_dot(g, points, px + a, py);
                grid_face_set_dot(g, d, 0);
                d = grid_get_dot(g, points, px + a + b, py + a);
                grid_face_set_dot(g, d, 1);
                d = grid_get_dot(g, points, px + b, py + a + b);
                grid_face_set_dot(g, d, 2);
                d = grid_get_dot(g, points, px, py + b);
                grid_face_set_dot(g, d, 3);
            } else {
                d = grid_get_dot(g, points, px + b, py);
                grid_face_set_dot(g, d, 0);
                d = grid_get_dot(g, points, px + a + b, py + b);
                grid_face_set_dot(g, d, 1);
                d = grid_get_dot(g, points, px + a, py + a + b);
                grid_face_set_dot(g, d, 2);
                d = grid_get_dot(g, points, px, py + a);
                grid_face_set_dot(g, d, 3);
            }

            /* generate up/down triangles */
            if (x > 0) {
                grid_face_add_new(g, 3);
                if ((x + y) % 2) {
                    d = grid_get_dot(g, points, px + a, py);
                    grid_face_set_dot(g, d, 0);
                    d = grid_get_dot(g, points, px, py + b);
                    grid_face_set_dot(g, d, 1);
                    d = grid_get_dot(g, points, px - a, py);
                    grid_face_set_dot(g, d, 2);
                } else {
                    d = grid_get_dot(g, points, px, py + a);
                    grid_face_set_dot(g, d, 0);
                    d = grid_get_dot(g, points, px + a, py + a + b);
                    grid_face_set_dot(g, d, 1);
                    d = grid_get_dot(g, points, px - a, py + a + b);
                    grid_face_set_dot(g, d, 2);
                }
            }

            /* generate left/right triangles */
            if (y > 0) {
                grid_face_add_new(g, 3);
                if ((x + y) % 2) {
                    d = grid_get_dot(g, points, px + a, py);
                    grid_face_set_dot(g, d, 0);
                    d = grid_get_dot(g, points, px + a + b, py - a);
                    grid_face_set_dot(g, d, 1);
                    d = grid_get_dot(g, points, px + a + b, py + a);
                    grid_face_set_dot(g, d, 2);
                } else {
                    d = grid_get_dot(g, points, px, py - a);
                    grid_face_set_dot(g, d, 0);
                    d = grid_get_dot(g, points, px + b, py);
                    grid_face_set_dot(g, d, 1);
                    d = grid_get_dot(g, points, px, py + a);
                    grid_face_set_dot(g, d, 2);
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