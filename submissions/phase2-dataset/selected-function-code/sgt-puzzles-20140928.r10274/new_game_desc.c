static char *new_game_desc(const game_params *params, random_state *rs,
			   char **aux, int interactive)
{
    int gap, n, i, x;
    int x1, x2, p1, p2, parity;
    int *tiles, *used;
    char *ret;
    int retlen;

    n = params->w * params->h;

    tiles = snewn(n, int);
    used = snewn(n, int);

    for (i = 0; i < n; i++) {
        tiles[i] = -1;
        used[i] = FALSE;
    }

    gap = random_upto(rs, n);
    tiles[gap] = 0;
    used[0] = TRUE;

    /*
     * Place everything else except the last two tiles.
     */
    for (x = 0, i = n-1; i > 2; i--) {
        int k = random_upto(rs, i);
        int j;

        for (j = 0; j < n; j++)
            if (!used[j] && (k-- == 0))
                break;

        assert(j < n && !used[j]);
        used[j] = TRUE;

        while (tiles[x] >= 0)
            x++;
        assert(x < n);
        tiles[x] = j;
    }

    /*
     * Find the last two locations, and the last two pieces.
     */
    while (tiles[x] >= 0)
        x++;
    assert(x < n);
    x1 = x;
    x++;
    while (tiles[x] >= 0)
        x++;
    assert(x < n);
    x2 = x;

    for (i = 0; i < n; i++)
        if (!used[i])
            break;
    p1 = i;
    for (i = p1+1; i < n; i++)
        if (!used[i])
            break;
    p2 = i;

    /*
     * Determine the required parity of the overall permutation.
     * This is the XOR of:
     * 
     * 	- The chessboard parity ((x^y)&1) of the gap square. The
     * 	  bottom right counts as even.
     * 
     *  - The parity of n. (The target permutation is 1,...,n-1,0
     *    rather than 0,...,n-1; this is a cyclic permutation of
     *    the starting point and hence is odd iff n is even.)
     */
    parity = ((X(params, gap) - (params->w-1)) ^
	      (Y(params, gap) - (params->h-1)) ^
	      (n+1)) & 1;

    /*
     * Try the last two tiles one way round. If that fails, swap
     * them.
     */
    tiles[x1] = p1;
    tiles[x2] = p2;
    if (perm_parity(tiles, n) != parity) {
        tiles[x1] = p2;
        tiles[x2] = p1;
        assert(perm_parity(tiles, n) == parity);
    }

    /*
     * Now construct the game description, by describing the tile
     * array as a simple sequence of comma-separated integers.
     */
    ret = NULL;
    retlen = 0;
    for (i = 0; i < n; i++) {
        char buf[80];
        int k;

        k = sprintf(buf, "%d,", tiles[i]);

        ret = sresize(ret, retlen + k + 1, char);
        strcpy(ret + retlen, buf);
        retlen += k;
    }
    ret[retlen-1] = '\0';              /* delete last comma */

    sfree(tiles);
    sfree(used);

    return ret;
}