static grid *grid_new_floret(int width, int height, const char *desc)
{
    int x, y;
    /* Vectors for sides; weird numbers needed to keep puzzle aligned with window
     * -py/px is close to tan(30 - atan(sqrt(3)/9))
     * using py=26 makes everything lean to the left, rather than right
     */
    int px = FLORET_PX, py = FLORET_PY;         /* |( 75, -26)| = 79.43 */
    int qx = 4*px/5, qy = -py*2;                /* |( 60,  52)| = 79.40 */
    int rx = qx-px, ry = qy-py;                 /* |(-15,  78)| = 79.38 */

    /* Upper bounds - don't have to be exact */
    int max_faces = 6 * width * height;
    int max_dots = 9 * (width + 1) * (height + 1);

    tree234 *points;

    grid *g = grid_empty();
    g->tilesize = FLORET_TILESIZE;
    g->faces = snewn(max_faces, grid_face);
    g->dots = snewn(max_dots, grid_dot);

    points = newtree234(grid_point_cmp_fn);

    /* generate pentagonal faces */
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            grid_dot *d;
            /* face centre */
            int cx = (6*px+3*qx)/2 * x;
            int cy = (4*py-5*qy) * y;
            if (x % 2)
                cy -= (4*py-5*qy)/2;
            else if (y && y == height-1)
                continue; /* make better looking grids?  try 3x3 for instance */

            grid_face_add_new(g, 5);
            d = grid_get_dot(g, points, cx        , cy        ); grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, cx+2*rx   , cy+2*ry   ); grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, cx+2*rx+qx, cy+2*ry+qy); grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, cx+2*qx+rx, cy+2*qy+ry); grid_face_set_dot(g, d, 3);
            d = grid_get_dot(g, points, cx+2*qx   , cy+2*qy   ); grid_face_set_dot(g, d, 4);

            grid_face_add_new(g, 5);
            d = grid_get_dot(g, points, cx        , cy        ); grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, cx+2*qx   , cy+2*qy   ); grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, cx+2*qx+px, cy+2*qy+py); grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, cx+2*px+qx, cy+2*py+qy); grid_face_set_dot(g, d, 3);
            d = grid_get_dot(g, points, cx+2*px   , cy+2*py   ); grid_face_set_dot(g, d, 4);

            grid_face_add_new(g, 5);
            d = grid_get_dot(g, points, cx        , cy        ); grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, cx+2*px   , cy+2*py   ); grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, cx+2*px-rx, cy+2*py-ry); grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, cx-2*rx+px, cy-2*ry+py); grid_face_set_dot(g, d, 3);
            d = grid_get_dot(g, points, cx-2*rx   , cy-2*ry   ); grid_face_set_dot(g, d, 4);

            grid_face_add_new(g, 5);
            d = grid_get_dot(g, points, cx        , cy        ); grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, cx-2*rx   , cy-2*ry   ); grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, cx-2*rx-qx, cy-2*ry-qy); grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, cx-2*qx-rx, cy-2*qy-ry); grid_face_set_dot(g, d, 3);
            d = grid_get_dot(g, points, cx-2*qx   , cy-2*qy   ); grid_face_set_dot(g, d, 4);

            grid_face_add_new(g, 5);
            d = grid_get_dot(g, points, cx        , cy        ); grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, cx-2*qx   , cy-2*qy   ); grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, cx-2*qx-px, cy-2*qy-py); grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, cx-2*px-qx, cy-2*py-qy); grid_face_set_dot(g, d, 3);
            d = grid_get_dot(g, points, cx-2*px   , cy-2*py   ); grid_face_set_dot(g, d, 4);

            grid_face_add_new(g, 5);
            d = grid_get_dot(g, points, cx        , cy        ); grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, cx-2*px   , cy-2*py   ); grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, cx-2*px+rx, cy-2*py+ry); grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, cx+2*rx-px, cy+2*ry-py); grid_face_set_dot(g, d, 3);
            d = grid_get_dot(g, points, cx+2*rx   , cy+2*ry   ); grid_face_set_dot(g, d, 4);
        }
    }

    freetree234(points);
    assert(g->num_faces <= max_faces);
    assert(g->num_dots <= max_dots);

    grid_make_consistent(g);
    return g;
}