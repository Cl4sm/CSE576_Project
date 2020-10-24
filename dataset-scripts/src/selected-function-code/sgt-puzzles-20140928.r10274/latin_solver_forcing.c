int latin_solver_forcing(struct latin_solver *solver,
                         struct latin_solver_scratch *scratch)
{
    int o = solver->o;
#ifdef STANDALONE_SOLVER
    char **names = solver->names;
#endif
    int *bfsqueue = scratch->bfsqueue;
#ifdef STANDALONE_SOLVER
    int *bfsprev = scratch->bfsprev;
#endif
    unsigned char *number = scratch->grid;
    int *neighbours = scratch->neighbours;
    int x, y;

    for (y = 0; y < o; y++)
        for (x = 0; x < o; x++) {
            int count, t, n;

            /*
             * If this square doesn't have exactly two candidate
             * numbers, don't try it.
             *
             * In this loop we also sum the candidate numbers,
             * which is a nasty hack to allow us to quickly find
             * `the other one' (since we will shortly know there
             * are exactly two).
             */
            for (count = t = 0, n = 1; n <= o; n++)
                if (cube(x, y, n))
                    count++, t += n;
            if (count != 2)
                continue;

            /*
             * Now attempt a bfs for each candidate.
             */
            for (n = 1; n <= o; n++)
                if (cube(x, y, n)) {
                    int orign, currn, head, tail;

                    /*
                     * Begin a bfs.
                     */
                    orign = n;

                    memset(number, o+1, o*o);
                    head = tail = 0;
                    bfsqueue[tail++] = y*o+x;
#ifdef STANDALONE_SOLVER
                    bfsprev[y*o+x] = -1;
#endif
                    number[y*o+x] = t - n;

                    while (head < tail) {
                        int xx, yy, nneighbours, xt, yt, i;

                        xx = bfsqueue[head++];
                        yy = xx / o;
                        xx %= o;

                        currn = number[yy*o+xx];

                        /*
                         * Find neighbours of yy,xx.
                         */
                        nneighbours = 0;
                        for (yt = 0; yt < o; yt++)
                            neighbours[nneighbours++] = yt*o+xx;
                        for (xt = 0; xt < o; xt++)
                            neighbours[nneighbours++] = yy*o+xt;

                        /*
                         * Try visiting each of those neighbours.
                         */
                        for (i = 0; i < nneighbours; i++) {
                            int cc, tt, nn;

                            xt = neighbours[i] % o;
                            yt = neighbours[i] / o;

                            /*
                             * We need this square to not be
                             * already visited, and to include
                             * currn as a possible number.
                             */
                            if (number[yt*o+xt] <= o)
                                continue;
                            if (!cube(xt, yt, currn))
                                continue;

                            /*
                             * Don't visit _this_ square a second
                             * time!
                             */
                            if (xt == xx && yt == yy)
                                continue;

                            /*
                             * To continue with the bfs, we need
                             * this square to have exactly two
                             * possible numbers.
                             */
                            for (cc = tt = 0, nn = 1; nn <= o; nn++)
                                if (cube(xt, yt, nn))
                                    cc++, tt += nn;
                            if (cc == 2) {
                                bfsqueue[tail++] = yt*o+xt;
#ifdef STANDALONE_SOLVER
                                bfsprev[yt*o+xt] = yy*o+xx;
#endif
                                number[yt*o+xt] = tt - currn;
                            }

                            /*
                             * One other possibility is that this
                             * might be the square in which we can
                             * make a real deduction: if it's
                             * adjacent to x,y, and currn is equal
                             * to the original number we ruled out.
                             */
                            if (currn == orign &&
                                (xt == x || yt == y)) {
#ifdef STANDALONE_SOLVER
                                if (solver_show_working) {
                                    char *sep = "";
                                    int xl, yl;
                                    printf("%*sforcing chain, %s at ends of ",
                                           solver_recurse_depth*4, "",
					   names[orign-1]);
                                    xl = xx;
                                    yl = yy;
                                    while (1) {
                                        printf("%s(%d,%d)", sep, xl+1,
                                               yl+1);
                                        xl = bfsprev[yl*o+xl];
                                        if (xl < 0)
                                            break;
                                        yl = xl / o;
                                        xl %= o;
                                        sep = "-";
                                    }
                                    printf("\n%*s  ruling out %s at (%d,%d)\n",
                                           solver_recurse_depth*4, "",
                                           names[orign-1],
					   xt+1, yt+1);
                                }
#endif
                                cube(xt, yt, orign) = FALSE;
                                return 1;
                            }
                        }
                    }
                }
        }

    return 0;
}