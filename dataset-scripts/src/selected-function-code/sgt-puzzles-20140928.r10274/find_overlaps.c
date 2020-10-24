static int find_overlaps(int w, int h, int placement, int *set)
{
    int x, y, n;

    n = 0;                             /* number of returned placements */

    x = placement / 2;
    y = x / w;
    x %= w;

    if (placement & 1) {
        /*
         * Horizontal domino, indexed by its left end.
         */
        if (x > 0)
            set[n++] = placement-2;    /* horizontal domino to the left */
        if (y > 0)
            set[n++] = placement-2*w-1;/* vertical domino above left side */
        if (y+1 < h)
            set[n++] = placement-1;    /* vertical domino below left side */
        if (x+2 < w)
            set[n++] = placement+2;    /* horizontal domino to the right */
        if (y > 0)
            set[n++] = placement-2*w+2-1;/* vertical domino above right side */
        if (y+1 < h)
            set[n++] = placement+2-1;  /* vertical domino below right side */
    } else {
        /*
         * Vertical domino, indexed by its top end.
         */
        if (y > 0)
            set[n++] = placement-2*w;  /* vertical domino above */
        if (x > 0)
            set[n++] = placement-2+1;  /* horizontal domino left of top */
        if (x+1 < w)
            set[n++] = placement+1;    /* horizontal domino right of top */
        if (y+2 < h)
            set[n++] = placement+2*w;  /* vertical domino below */
        if (x > 0)
            set[n++] = placement-2+2*w+1;/* horizontal domino left of bottom */
        if (x+1 < w)
            set[n++] = placement+2*w+1;/* horizontal domino right of bottom */
    }

    return n;
}