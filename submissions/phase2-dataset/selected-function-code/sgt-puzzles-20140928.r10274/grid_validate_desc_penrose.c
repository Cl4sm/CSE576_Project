static char *grid_validate_desc_penrose(grid_type type, int width, int height,
                                        const char *desc)
{
    int tilesize = PENROSE_TILESIZE, startsz, depth, xoff, yoff, aoff, inner_radius;
    double outer_radius;
    int which = (type == GRID_PENROSE_P2 ? PENROSE_P2 : PENROSE_P3);
    grid *g;

    if (!desc)
        return "Missing grid description string.";

    penrose_calculate_size(which, tilesize, width, height,
                           &outer_radius, &startsz, &depth);
    inner_radius = (int)(outer_radius - sqrt(width*width + height*height));

    if (sscanf(desc, "G%d,%d,%d", &xoff, &yoff, &aoff) != 3)
        return "Invalid format grid description string.";

    if (sqrt(xoff*xoff + yoff*yoff) > inner_radius)
        return "Patch offset out of bounds.";
    if ((aoff % 36) != 0 || aoff < 0 || aoff >= 360)
        return "Angle offset out of bounds.";

    /*
     * Test-generate to ensure these parameters don't end us up with
     * no grid at all.
     */
    g = grid_new_penrose(width, height, which, desc);
    if (!g)
        return "Patch coordinates do not identify a usable grid fragment";
    grid_free(g);

    return NULL;
}