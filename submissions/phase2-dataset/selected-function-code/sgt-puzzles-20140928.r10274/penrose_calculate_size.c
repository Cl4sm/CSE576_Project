void penrose_calculate_size(int which, int tilesize, int w, int h,
                            double *required_radius, int *start_size, int *depth)
{
    double rradius, size;
    int n = 0;

    /*
     * Fudge factor to scale P2 and P3 tilings differently. This
     * doesn't seem to have much relevance to questions like the
     * average number of tiles per unit area; it's just aesthetic.
     */
    if (which == PENROSE_P2)
        tilesize = tilesize * 3 / 2;
    else
        tilesize = tilesize * 5 / 4;

    rradius = tilesize * 3.11 * sqrt((double)(w*w + h*h));
    size = tilesize;

    while ((size * INCIRCLE_RADIUS) < rradius) {
        n++;
        size = size * PHI;
    }

    *start_size = (int)size;
    *depth = n;
    *required_radius = rradius;
}