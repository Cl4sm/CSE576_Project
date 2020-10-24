static int solver(int w, int h, int n, int *grid, int *output)
{
    int wh = w*h, dc = DCOUNT(n);
    int *placements, *heads;
    int i, j, x, y, ret;

    /*
     * This array has one entry for every possible domino
     * placement. Vertical placements are indexed by their top
     * half, at (y*w+x)*2; horizontal placements are indexed by
     * their left half at (y*w+x)*2+1.
     * 
     * This array is used to link domino placements together into
     * linked lists, so that we can track all the possible
     * placements of each different domino. It's also used as a
     * quick means of looking up an individual placement to see
     * whether we still think it's possible. Actual values stored
     * in this array are -2 (placement not possible at all), -1
     * (end of list), or the array index of the next item.
     * 
     * Oh, and -3 for `not even valid', used for array indices
     * which don't even represent a plausible placement.
     */
    placements = snewn(2*wh, int);
    for (i = 0; i < 2*wh; i++)
        placements[i] = -3;            /* not even valid */

    /*
     * This array has one entry for every domino, and it is an
     * index into `placements' denoting the head of the placement
     * list for that domino.
     */
    heads = snewn(dc, int);
    for (i = 0; i < dc; i++)
        heads[i] = -1;

    /*
     * Set up the initial possibility lists by scanning the grid.
     */
    for (y = 0; y < h-1; y++)
        for (x = 0; x < w; x++) {
            int di = DINDEX(grid[y*w+x], grid[(y+1)*w+x]);
            placements[(y*w+x)*2] = heads[di];
            heads[di] = (y*w+x)*2;
        }
    for (y = 0; y < h; y++)
        for (x = 0; x < w-1; x++) {
            int di = DINDEX(grid[y*w+x], grid[y*w+(x+1)]);
            placements[(y*w+x)*2+1] = heads[di];
            heads[di] = (y*w+x)*2+1;
        }

#ifdef SOLVER_DIAGNOSTICS
    printf("before solver:\n");
    for (i = 0; i <= n; i++)
        for (j = 0; j <= i; j++) {
            int k, m;
            m = 0;
            printf("%2d [%d %d]:", DINDEX(i, j), i, j);
            for (k = heads[DINDEX(i,j)]; k >= 0; k = placements[k])
                printf(" %3d [%d,%d,%c]", k, k/2%w, k/2/w, k%2?'h':'v');
            printf("\n");
        }
#endif

    while (1) {
        int done_something = FALSE;

        /*
         * For each domino, look at its possible placements, and
         * for each placement consider the placements (of any
         * domino) it overlaps. Any placement overlapped by all
         * placements of this domino can be ruled out.
         * 
         * Each domino placement overlaps only six others, so we
         * need not do serious set theory to work this out.
         */
        for (i = 0; i < dc; i++) {
            int permset[6], permlen = 0, p;
            

            if (heads[i] == -1) {      /* no placement for this domino */
                ret = 0;               /* therefore puzzle is impossible */
                goto done;
            }
            for (j = heads[i]; j >= 0; j = placements[j]) {
                assert(placements[j] != -2);

                if (j == heads[i]) {
                    permlen = find_overlaps(w, h, j, permset);
                } else {
                    int tempset[6], templen, m, n, k;

                    templen = find_overlaps(w, h, j, tempset);

                    /*
                     * Pathetically primitive set intersection
                     * algorithm, which I'm only getting away with
                     * because I know my sets are bounded by a very
                     * small size.
                     */
                    for (m = n = 0; m < permlen; m++) {
                        for (k = 0; k < templen; k++)
                            if (tempset[k] == permset[m])
                                break;
                        if (k < templen)
                            permset[n++] = permset[m];
                    }
                    permlen = n;
                }
            }
            for (p = 0; p < permlen; p++) {
                j = permset[p];
                if (placements[j] != -2) {
                    int p1, p2, di;

                    done_something = TRUE;

                    /*
                     * Rule out this placement. First find what
                     * domino it is...
                     */
                    p1 = j / 2;
                    p2 = (j & 1) ? p1 + 1 : p1 + w;
                    di = DINDEX(grid[p1], grid[p2]);
#ifdef SOLVER_DIAGNOSTICS
                    printf("considering domino %d: ruling out placement %d"
                           " for %d\n", i, j, di);
#endif

                    /*
                     * ... then walk that domino's placement list,
                     * removing this placement when we find it.
                     */
                    if (heads[di] == j)
                        heads[di] = placements[j];
                    else {
                        int k = heads[di];
                        while (placements[k] != -1 && placements[k] != j)
                            k = placements[k];
                        assert(placements[k] == j);
                        placements[k] = placements[j];
                    }
                    placements[j] = -2;
                }
            }
        }

        /*
         * For each square, look at the available placements
         * involving that square. If all of them are for the same
         * domino, then rule out any placements for that domino
         * _not_ involving this square.
         */
        for (i = 0; i < wh; i++) {
            int list[4], k, n, adi;

            x = i % w;
            y = i / w;

            j = 0;
            if (x > 0)
                list[j++] = 2*(i-1)+1;
            if (x+1 < w)
                list[j++] = 2*i+1;
            if (y > 0)
                list[j++] = 2*(i-w);
            if (y+1 < h)
                list[j++] = 2*i;

            for (n = k = 0; k < j; k++)
                if (placements[list[k]] >= -1)
                    list[n++] = list[k];

            adi = -1;

            for (j = 0; j < n; j++) {
                int p1, p2, di;
                k = list[j];

                p1 = k / 2;
                p2 = (k & 1) ? p1 + 1 : p1 + w;
                di = DINDEX(grid[p1], grid[p2]);

                if (adi == -1)
                    adi = di;
                if (adi != di)
                    break;
            }

            if (j == n) {
                int nn;

                assert(adi >= 0);
                /*
                 * We've found something. All viable placements
                 * involving this square are for domino `adi'. If
                 * the current placement list for that domino is
                 * longer than n, reduce it to precisely this
                 * placement list and we've done something.
                 */
                nn = 0;
                for (k = heads[adi]; k >= 0; k = placements[k])
                    nn++;
                if (nn > n) {
                    done_something = TRUE;
#ifdef SOLVER_DIAGNOSTICS
                    printf("considering square %d,%d: reducing placements "
                           "of domino %d\n", x, y, adi);
#endif
                    /*
                     * Set all other placements on the list to
                     * impossible.
                     */
                    k = heads[adi];
                    while (k >= 0) {
                        int tmp = placements[k];
                        placements[k] = -2;
                        k = tmp;
                    }
                    /*
                     * Set up the new list.
                     */
                    heads[adi] = list[0];
                    for (k = 0; k < n; k++)
                        placements[list[k]] = (k+1 == n ? -1 : list[k+1]);
                }
            }
        }

        if (!done_something)
            break;
    }

#ifdef SOLVER_DIAGNOSTICS
    printf("after solver:\n");
    for (i = 0; i <= n; i++)
        for (j = 0; j <= i; j++) {
            int k, m;
            m = 0;
            printf("%2d [%d %d]:", DINDEX(i, j), i, j);
            for (k = heads[DINDEX(i,j)]; k >= 0; k = placements[k])
                printf(" %3d [%d,%d,%c]", k, k/2%w, k/2/w, k%2?'h':'v');
            printf("\n");
        }
#endif

    ret = 1;
    for (i = 0; i < wh*2; i++) {
        if (placements[i] == -2) {
            if (output)
                output[i] = -1;        /* ruled out */
        } else if (placements[i] != -3) {
            int p1, p2, di;

            p1 = i / 2;
            p2 = (i & 1) ? p1 + 1 : p1 + w;
            di = DINDEX(grid[p1], grid[p2]);

            if (i == heads[di] && placements[i] == -1) {
                if (output)
                    output[i] = 1;     /* certain */
            } else {
                if (output)
                    output[i] = 0;     /* uncertain */
                ret = 2;
            }
        }
    }

    done:
    /*
     * Free working data.
     */
    sfree(placements);
    sfree(heads);

    return ret;
}