static char *grid_new_desc_penrose(grid_type type, int width, int height, random_state *rs)
{
    int tilesize = PENROSE_TILESIZE, startsz, depth, xoff, yoff, aoff;
    double outer_radius;
    int inner_radius;
    char gd[255];
    int which = (type == GRID_PENROSE_P2 ? PENROSE_P2 : PENROSE_P3);
    grid *g;

    while (1) {
        /* We want to produce a random bit of penrose tiling, so we
         * calculate a random offset (within the patch that penrose.c
         * calculates for us) and an angle (multiple of 36) to rotate
         * the patch. */

        penrose_calculate_size(which, tilesize, width, height,
                               &outer_radius, &startsz, &depth);

        /* Calculate radius of (circumcircle of) patch, subtract from
         * radius calculated. */
        inner_radius = (int)(outer_radius - sqrt(width*width + height*height));

        /* Pick a random offset (the easy way: choose within outer
         * square, discarding while it's outside the circle) */
        do {
            xoff = random_upto(rs, 2*inner_radius) - inner_radius;
            yoff = random_upto(rs, 2*inner_radius) - inner_radius;
        } while (sqrt(xoff*xoff+yoff*yoff) > inner_radius);

        aoff = random_upto(rs, 360/36) * 36;

        debug(("grid_desc: ts %d, %dx%d patch, orad %2.2f irad %d",
               tilesize, width, height, outer_radius, inner_radius));
        debug(("    -> xoff %d yoff %d aoff %d", xoff, yoff, aoff));

        sprintf(gd, "G%d,%d,%d", xoff, yoff, aoff);

        /*
         * Now test-generate our grid, to make sure it actually
         * produces something.
         */
        g = grid_new_penrose(width, height, which, gd);
        if (g) {
            grid_free(g);
            break;
        }
        /* If not, go back to the top of this while loop and try again
         * with a different random offset. */
    }

    return dupstr(gd);
}