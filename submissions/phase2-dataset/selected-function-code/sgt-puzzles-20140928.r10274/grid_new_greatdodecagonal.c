static grid *grid_new_greatdodecagonal(int width, int height, const char *desc)
{
    int x, y;
    /* Vector for side of triangle - ratio is close to sqrt(3) */
    int a = DODEC_A;
    int b = DODEC_B;

    /* Upper bounds - don't have to be exact */
    int max_faces = 30 * width * height;
    int max_dots = 200 * width * height;

    tree234 *points;

    grid *g = grid_empty();
    g->tilesize = DODEC_TILESIZE;
    g->faces = snewn(max_faces, grid_face);
    g->dots = snewn(max_dots, grid_dot);

    points = newtree234(grid_point_cmp_fn);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            grid_dot *d;
            /* centre of dodecagon */
            int px = (6*a + 2*b) * x;
            int py = (3*a + 3*b) * y;
            if (y % 2)
                px += 3*a + b;

            /* dodecagon */
            grid_face_add_new(g, 12);
            d = grid_get_dot(g, points, px + (  a    ), py - (2*a + b)); grid_face_set_dot(g, d, 0);
            d = grid_get_dot(g, points, px + (  a + b), py - (  a + b)); grid_face_set_dot(g, d, 1);
            d = grid_get_dot(g, points, px + (2*a + b), py - (  a    )); grid_face_set_dot(g, d, 2);
            d = grid_get_dot(g, points, px + (2*a + b), py + (  a    )); grid_face_set_dot(g, d, 3);
            d = grid_get_dot(g, points, px + (  a + b), py + (  a + b)); grid_face_set_dot(g, d, 4);
            d = grid_get_dot(g, points, px + (  a    ), py + (2*a + b)); grid_face_set_dot(g, d, 5);
            d = grid_get_dot(g, points, px - (  a    ), py + (2*a + b)); grid_face_set_dot(g, d, 6);
            d = grid_get_dot(g, points, px - (  a + b), py + (  a + b)); grid_face_set_dot(g, d, 7);
            d = grid_get_dot(g, points, px - (2*a + b), py + (  a    )); grid_face_set_dot(g, d, 8);
            d = grid_get_dot(g, points, px - (2*a + b), py - (  a    )); grid_face_set_dot(g, d, 9);
            d = grid_get_dot(g, points, px - (  a + b), py - (  a + b)); grid_face_set_dot(g, d, 10);
            d = grid_get_dot(g, points, px - (  a    ), py - (2*a + b)); grid_face_set_dot(g, d, 11);

            /* hexagon below dodecagon */
	    if (y < height - 1 && (x < width - 1 || !(y % 2)) && (x > 0 || (y % 2))) {
	      	grid_face_add_new(g, 6);
	      	d = grid_get_dot(g, points, px +   a, py + (2*a +   b)); grid_face_set_dot(g, d, 0);
	      	d = grid_get_dot(g, points, px + 2*a, py + (2*a + 2*b)); grid_face_set_dot(g, d, 1);
	      	d = grid_get_dot(g, points, px +   a, py + (2*a + 3*b)); grid_face_set_dot(g, d, 2);
	      	d = grid_get_dot(g, points, px -   a, py + (2*a + 3*b)); grid_face_set_dot(g, d, 3);
	      	d = grid_get_dot(g, points, px - 2*a, py + (2*a + 2*b)); grid_face_set_dot(g, d, 4);
	      	d = grid_get_dot(g, points, px -   a, py + (2*a +   b)); grid_face_set_dot(g, d, 5);
	    }

            /* hexagon above dodecagon */
	    if (y && (x < width - 1 || !(y % 2)) && (x > 0 || (y % 2))) {
	      	grid_face_add_new(g, 6);
	      	d = grid_get_dot(g, points, px -   a, py - (2*a +   b)); grid_face_set_dot(g, d, 0);
	      	d = grid_get_dot(g, points, px - 2*a, py - (2*a + 2*b)); grid_face_set_dot(g, d, 1);
	      	d = grid_get_dot(g, points, px -   a, py - (2*a + 3*b)); grid_face_set_dot(g, d, 2);
	      	d = grid_get_dot(g, points, px +   a, py - (2*a + 3*b)); grid_face_set_dot(g, d, 3);
	      	d = grid_get_dot(g, points, px + 2*a, py - (2*a + 2*b)); grid_face_set_dot(g, d, 4);
	      	d = grid_get_dot(g, points, px +   a, py - (2*a +   b)); grid_face_set_dot(g, d, 5);
	    }

            /* square on right of dodecagon */
	    if (x < width - 1) {
	      	grid_face_add_new(g, 4);
	      	d = grid_get_dot(g, points, px + 2*a + b, py - a); grid_face_set_dot(g, d, 0);
	      	d = grid_get_dot(g, points, px + 4*a + b, py - a); grid_face_set_dot(g, d, 1);
	      	d = grid_get_dot(g, points, px + 4*a + b, py + a); grid_face_set_dot(g, d, 2);
	      	d = grid_get_dot(g, points, px + 2*a + b, py + a); grid_face_set_dot(g, d, 3);
	    }

            /* square on top right of dodecagon */
	    if (y && (x < width - 1 || !(y % 2))) {
	      	grid_face_add_new(g, 4);
	      	d = grid_get_dot(g, points, px + (  a    ), py - (2*a +   b)); grid_face_set_dot(g, d, 0);
		d = grid_get_dot(g, points, px + (2*a    ), py - (2*a + 2*b)); grid_face_set_dot(g, d, 1);
		d = grid_get_dot(g, points, px + (2*a + b), py - (  a + 2*b)); grid_face_set_dot(g, d, 2);
		d = grid_get_dot(g, points, px + (  a + b), py - (  a +   b)); grid_face_set_dot(g, d, 3);
	    }

            /* square on top left of dodecagon */
	    if (y && (x || (y % 2))) {
	      	grid_face_add_new(g, 4);
		d = grid_get_dot(g, points, px - (  a + b), py - (  a +   b)); grid_face_set_dot(g, d, 0);
		d = grid_get_dot(g, points, px - (2*a + b), py - (  a + 2*b)); grid_face_set_dot(g, d, 1);
		d = grid_get_dot(g, points, px - (2*a    ), py - (2*a + 2*b)); grid_face_set_dot(g, d, 2);
	      	d = grid_get_dot(g, points, px - (  a    ), py - (2*a +   b)); grid_face_set_dot(g, d, 3);
	    }
	}
    }

    freetree234(points);
    assert(g->num_faces <= max_faces);
    assert(g->num_dots <= max_dots);

    grid_make_consistent(g);
    return g;
}