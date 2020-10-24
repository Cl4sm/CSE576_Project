static int slant_solve(int w, int h, const signed char *clues,
		       signed char *soln, struct solver_scratch *sc,
		       int difficulty)
{
    int W = w+1, H = h+1;
    int x, y, i, j;
    int done_something;

    /*
     * Clear the output.
     */
    memset(soln, 0, w*h);

    sc->clues = clues;

    /*
     * Establish a disjoint set forest for tracking connectedness
     * between grid points.
     */
    dsf_init(sc->connected, W*H);

    /*
     * Establish a disjoint set forest for tracking which squares
     * are known to slant in the same direction.
     */
    dsf_init(sc->equiv, w*h);

    /*
     * Clear the slashval array.
     */
    memset(sc->slashval, 0, w*h);

    /*
     * Set up the vbitmap array. Initially all types of v are possible.
     */
    memset(sc->vbitmap, 0xF, w*h);

    /*
     * Initialise the `exits' and `border' arrays. These are used
     * to do second-order loop avoidance: the dual of the no loops
     * constraint is that every point must be somehow connected to
     * the border of the grid (otherwise there would be a solid
     * loop around it which prevented this).
     * 
     * I define a `dead end' to be a connected group of points
     * which contains no border point, and which can form at most
     * one new connection outside itself. Then I forbid placing an
     * edge so that it connects together two dead-end groups, since
     * this would yield a non-border-connected isolated subgraph
     * with no further scope to extend it.
     */
    for (y = 0; y < H; y++)
	for (x = 0; x < W; x++) {
	    if (y == 0 || y == H-1 || x == 0 || x == W-1)
		sc->border[y*W+x] = TRUE;
	    else
		sc->border[y*W+x] = FALSE;

	    if (clues[y*W+x] < 0)
		sc->exits[y*W+x] = 4;
	    else
		sc->exits[y*W+x] = clues[y*W+x];
	}

    /*
     * Repeatedly try to deduce something until we can't.
     */
    do {
	done_something = FALSE;

	/*
	 * Any clue point with the number of remaining lines equal
	 * to zero or to the number of remaining undecided
	 * neighbouring squares can be filled in completely.
	 */
	for (y = 0; y < H; y++)
	    for (x = 0; x < W; x++) {
		struct {
		    int pos, slash;
		} neighbours[4];
		int nneighbours;
		int nu, nl, c, s, eq, eq2, last, meq, mj1, mj2;

		if ((c = clues[y*W+x]) < 0)
		    continue;

		/*
		 * We have a clue point. Start by listing its
		 * neighbouring squares, in order around the point,
		 * together with the type of slash that would be
		 * required in that square to connect to the point.
		 */
		nneighbours = 0;
		if (x > 0 && y > 0) {
		    neighbours[nneighbours].pos = (y-1)*w+(x-1);
		    neighbours[nneighbours].slash = -1;
		    nneighbours++;
		}
		if (x > 0 && y < h) {
		    neighbours[nneighbours].pos = y*w+(x-1);
		    neighbours[nneighbours].slash = +1;
		    nneighbours++;
		}
		if (x < w && y < h) {
		    neighbours[nneighbours].pos = y*w+x;
		    neighbours[nneighbours].slash = -1;
		    nneighbours++;
		}
		if (x < w && y > 0) {
		    neighbours[nneighbours].pos = (y-1)*w+x;
		    neighbours[nneighbours].slash = +1;
		    nneighbours++;
		}

		/*
		 * Count up the number of undecided neighbours, and
		 * also the number of lines already present.
		 *
		 * If we're not on DIFF_EASY, then in this loop we
		 * also track whether we've seen two adjacent empty
		 * squares belonging to the same equivalence class
		 * (meaning they have the same type of slash). If
		 * so, we count them jointly as one line.
		 */
		nu = 0;
		nl = c;
		last = neighbours[nneighbours-1].pos;
		if (soln[last] == 0)
		    eq = dsf_canonify(sc->equiv, last);
		else
		    eq = -1;
		meq = mj1 = mj2 = -1;
		for (i = 0; i < nneighbours; i++) {
		    j = neighbours[i].pos;
		    s = neighbours[i].slash;
		    if (soln[j] == 0) {
			nu++;	       /* undecided */
			if (meq < 0 && difficulty > DIFF_EASY) {
			    eq2 = dsf_canonify(sc->equiv, j);
			    if (eq == eq2 && last != j) {
				/*
				 * We've found an equivalent pair.
				 * Mark it. This also inhibits any
				 * further equivalence tracking
				 * around this square, since we can
				 * only handle one pair (and in
				 * particular we want to avoid
				 * being misled by two overlapping
				 * equivalence pairs).
				 */
				meq = eq;
				mj1 = last;
				mj2 = j;
				nl--;   /* count one line */
				nu -= 2;   /* and lose two undecideds */
			    } else
				eq = eq2;
			}
		    } else {
			eq = -1;
			if (soln[j] == s)
			    nl--;      /* here's a line */
		    }
		    last = j;
		}

		/*
		 * Check the counts.
		 */
		if (nl < 0 || nl > nu) {
		    /*
		     * No consistent value for this at all!
		     */
#ifdef SOLVER_DIAGNOSTICS
		    if (verbose)
			printf("need %d / %d lines around clue point at %d,%d!\n",
			       nl, nu, x, y);
#endif
		    return 0;	       /* impossible */
		}

		if (nu > 0 && (nl == 0 || nl == nu)) {
#ifdef SOLVER_DIAGNOSTICS
		    if (verbose) {
			if (meq >= 0)
			    printf("partially (since %d,%d == %d,%d) ",
				   mj1%w, mj1/w, mj2%w, mj2/w);
			printf("%s around clue point at %d,%d\n",
			       nl ? "filling" : "emptying", x, y);
		    }
#endif
		    for (i = 0; i < nneighbours; i++) {
			j = neighbours[i].pos;
			s = neighbours[i].slash;
			if (soln[j] == 0 && j != mj1 && j != mj2)
			    fill_square(w, h, j%w, j/w, (nl ? s : -s), soln,
					sc->connected, sc);
		    }

		    done_something = TRUE;
		} else if (nu == 2 && nl == 1 && difficulty > DIFF_EASY) {
		    /*
		     * If we have precisely two undecided squares
		     * and precisely one line to place between
		     * them, _and_ those squares are adjacent, then
		     * we can mark them as equivalent to one
		     * another.
		     * 
		     * This even applies if meq >= 0: if we have a
		     * 2 clue point and two of its neighbours are
		     * already marked equivalent, we can indeed
		     * mark the other two as equivalent.
		     * 
		     * We don't bother with this on DIFF_EASY,
		     * since we wouldn't have used the results
		     * anyway.
		     */
		    last = -1;
		    for (i = 0; i < nneighbours; i++) {
			j = neighbours[i].pos;
			if (soln[j] == 0 && j != mj1 && j != mj2) {
			    if (last < 0)
				last = i;
			    else if (last == i-1 || (last == 0 && i == 3))
				break; /* found a pair */
			}
		    }
		    if (i < nneighbours) {
			int sv1, sv2;

			assert(last >= 0);
			/*
			 * neighbours[last] and neighbours[i] are
			 * the pair. Mark them equivalent.
			 */
#ifdef SOLVER_DIAGNOSTICS
			if (verbose) {
			    if (meq >= 0)
				printf("since %d,%d == %d,%d, ",
				       mj1%w, mj1/w, mj2%w, mj2/w);
			}
#endif
			mj1 = neighbours[last].pos;
			mj2 = neighbours[i].pos;
#ifdef SOLVER_DIAGNOSTICS
			if (verbose)
			    printf("clue point at %d,%d implies %d,%d == %d,"
				   "%d\n", x, y, mj1%w, mj1/w, mj2%w, mj2/w);
#endif
			mj1 = dsf_canonify(sc->equiv, mj1);
			sv1 = sc->slashval[mj1];
			mj2 = dsf_canonify(sc->equiv, mj2);
			sv2 = sc->slashval[mj2];
			if (sv1 != 0 && sv2 != 0 && sv1 != sv2) {
#ifdef SOLVER_DIAGNOSTICS
			    if (verbose)
				printf("merged two equivalence classes with"
				       " different slash values!\n");
#endif
			    return 0;
			}
			sv1 = sv1 ? sv1 : sv2;
			dsf_merge(sc->equiv, mj1, mj2);
			mj1 = dsf_canonify(sc->equiv, mj1);
			sc->slashval[mj1] = sv1;
		    }
		}
	    }

	if (done_something)
	    continue;

	/*
	 * Failing that, we now apply the second condition, which
	 * is that no square may be filled in such a way as to form
	 * a loop. Also in this loop (since it's over squares
	 * rather than points), we check slashval to see if we've
	 * already filled in another square in the same equivalence
	 * class.
	 * 
	 * The slashval check is disabled on DIFF_EASY, as is dead
	 * end avoidance. Only _immediate_ loop avoidance remains.
	 */
	for (y = 0; y < h; y++)
	    for (x = 0; x < w; x++) {
		int fs, bs, v;
		int c1, c2;
#ifdef SOLVER_DIAGNOSTICS
		char *reason = "<internal error>";
#endif

		if (soln[y*w+x])
		    continue;	       /* got this one already */

		fs = FALSE;
		bs = FALSE;

		if (difficulty > DIFF_EASY)
		    v = sc->slashval[dsf_canonify(sc->equiv, y*w+x)];
		else
		    v = 0;

		/*
		 * Try to rule out connectivity between (x,y) and
		 * (x+1,y+1); if successful, we will deduce that we
		 * must have a forward slash.
		 */
		c1 = dsf_canonify(sc->connected, y*W+x);
		c2 = dsf_canonify(sc->connected, (y+1)*W+(x+1));
		if (c1 == c2) {
		    fs = TRUE;
#ifdef SOLVER_DIAGNOSTICS
		    reason = "simple loop avoidance";
#endif
		}
		if (difficulty > DIFF_EASY &&
		    !sc->border[c1] && !sc->border[c2] &&
		    sc->exits[c1] <= 1 && sc->exits[c2] <= 1) {
		    fs = TRUE;
#ifdef SOLVER_DIAGNOSTICS
		    reason = "dead end avoidance";
#endif
		}
		if (v == +1) {
		    fs = TRUE;
#ifdef SOLVER_DIAGNOSTICS
		    reason = "equivalence to an already filled square";
#endif
		}

		/*
		 * Now do the same between (x+1,y) and (x,y+1), to
		 * see if we are required to have a backslash.
		 */
		c1 = dsf_canonify(sc->connected, y*W+(x+1));
		c2 = dsf_canonify(sc->connected, (y+1)*W+x);
		if (c1 == c2) {
		    bs = TRUE;
#ifdef SOLVER_DIAGNOSTICS
		    reason = "simple loop avoidance";
#endif
		}
		if (difficulty > DIFF_EASY &&
		    !sc->border[c1] && !sc->border[c2] &&
		    sc->exits[c1] <= 1 && sc->exits[c2] <= 1) {
		    bs = TRUE;
#ifdef SOLVER_DIAGNOSTICS
		    reason = "dead end avoidance";
#endif
		}
		if (v == -1) {
		    bs = TRUE;
#ifdef SOLVER_DIAGNOSTICS
		    reason = "equivalence to an already filled square";
#endif
		}

		if (fs && bs) {
		    /*
		     * No consistent value for this at all!
		     */
#ifdef SOLVER_DIAGNOSTICS
		    if (verbose)
			printf("%d,%d has no consistent slash!\n", x, y);
#endif
		    return 0;          /* impossible */
		}

		if (fs) {
#ifdef SOLVER_DIAGNOSTICS
		    if (verbose)
			printf("employing %s\n", reason);
#endif
		    fill_square(w, h, x, y, +1, soln, sc->connected, sc);
		    done_something = TRUE;
		} else if (bs) {
#ifdef SOLVER_DIAGNOSTICS
		    if (verbose)
			printf("employing %s\n", reason);
#endif
		    fill_square(w, h, x, y, -1, soln, sc->connected, sc);
		    done_something = TRUE;
		}
	    }

	if (done_something)
	    continue;

        /*
         * Now see what we can do with the vbitmap array. All
         * vbitmap deductions are disabled at Easy level.
         */
        if (difficulty <= DIFF_EASY)
            continue;

	for (y = 0; y < h; y++)
	    for (x = 0; x < w; x++) {
                int s, c;

                /*
                 * Any line already placed in a square must rule
                 * out any type of v which contradicts it.
                 */
                if ((s = soln[y*w+x]) != 0) {
                    if (x > 0)
                        done_something |=
                        vbitmap_clear(w, h, sc, x-1, y, (s < 0 ? 0x1 : 0x2),
                                      "contradicts known edge at (%d,%d)",x,y);
                    if (x+1 < w)
                        done_something |=
                        vbitmap_clear(w, h, sc, x, y, (s < 0 ? 0x2 : 0x1),
                                      "contradicts known edge at (%d,%d)",x,y);
                    if (y > 0)
                        done_something |=
                        vbitmap_clear(w, h, sc, x, y-1, (s < 0 ? 0x4 : 0x8),
                                      "contradicts known edge at (%d,%d)",x,y);
                    if (y+1 < h)
                        done_something |=
                        vbitmap_clear(w, h, sc, x, y, (s < 0 ? 0x8 : 0x4),
                                      "contradicts known edge at (%d,%d)",x,y);
                }

                /*
                 * If both types of v are ruled out for a pair of
                 * adjacent squares, mark them as equivalent.
                 */
                if (x+1 < w && !(sc->vbitmap[y*w+x] & 0x3)) {
                    int n1 = y*w+x, n2 = y*w+(x+1);
                    if (dsf_canonify(sc->equiv, n1) !=
                        dsf_canonify(sc->equiv, n2)) {
                        dsf_merge(sc->equiv, n1, n2);
                        done_something = TRUE;
#ifdef SOLVER_DIAGNOSTICS
                        if (verbose)
                            printf("(%d,%d) and (%d,%d) must be equivalent"
                                   " because both v-shapes are ruled out\n",
                                   x, y, x+1, y);
#endif
                    }
                }
                if (y+1 < h && !(sc->vbitmap[y*w+x] & 0xC)) {
                    int n1 = y*w+x, n2 = (y+1)*w+x;
                    if (dsf_canonify(sc->equiv, n1) !=
                        dsf_canonify(sc->equiv, n2)) {
                        dsf_merge(sc->equiv, n1, n2);
                        done_something = TRUE;
#ifdef SOLVER_DIAGNOSTICS
                        if (verbose)
                            printf("(%d,%d) and (%d,%d) must be equivalent"
                                   " because both v-shapes are ruled out\n",
                                   x, y, x, y+1);
#endif
                    }
                }

                /*
                 * The remaining work in this loop only works
                 * around non-edge clue points.
                 */
                if (y == 0 || x == 0)
                    continue;
		if ((c = clues[y*W+x]) < 0)
		    continue;

                /*
                 * x,y marks a clue point not on the grid edge. See
                 * if this clue point allows us to rule out any v
                 * shapes.
                 */

                if (c == 1) {
                    /*
                     * A 1 clue can never have any v shape pointing
                     * at it.
                     */
                    done_something |=
                        vbitmap_clear(w, h, sc, x-1, y-1, 0x5,
                                      "points at 1 clue at (%d,%d)", x, y);
                    done_something |=
                        vbitmap_clear(w, h, sc, x-1, y, 0x2,
                                      "points at 1 clue at (%d,%d)", x, y);
                    done_something |=
                        vbitmap_clear(w, h, sc, x, y-1, 0x8,
                                      "points at 1 clue at (%d,%d)", x, y);
                } else if (c == 3) {
                    /*
                     * A 3 clue can never have any v shape pointing
                     * away from it.
                     */
                    done_something |=
                        vbitmap_clear(w, h, sc, x-1, y-1, 0xA,
                                      "points away from 3 clue at (%d,%d)", x, y);
                    done_something |=
                        vbitmap_clear(w, h, sc, x-1, y, 0x1,
                                      "points away from 3 clue at (%d,%d)", x, y);
                    done_something |=
                        vbitmap_clear(w, h, sc, x, y-1, 0x4,
                                      "points away from 3 clue at (%d,%d)", x, y);
                } else if (c == 2) {
                    /*
                     * If a 2 clue has any kind of v ruled out on
                     * one side of it, the same v is ruled out on
                     * the other side.
                     */
                    done_something |=
                        vbitmap_clear(w, h, sc, x-1, y-1,
                                      (sc->vbitmap[(y  )*w+(x-1)] & 0x3) ^ 0x3,
                                      "propagated by 2 clue at (%d,%d)", x, y);
                    done_something |=
                        vbitmap_clear(w, h, sc, x-1, y-1,
                                      (sc->vbitmap[(y-1)*w+(x  )] & 0xC) ^ 0xC,
                                      "propagated by 2 clue at (%d,%d)", x, y);
                    done_something |=
                        vbitmap_clear(w, h, sc, x-1, y,
                                      (sc->vbitmap[(y-1)*w+(x-1)] & 0x3) ^ 0x3,
                                      "propagated by 2 clue at (%d,%d)", x, y);
                    done_something |=
                        vbitmap_clear(w, h, sc, x, y-1,
                                      (sc->vbitmap[(y-1)*w+(x-1)] & 0xC) ^ 0xC,
                                      "propagated by 2 clue at (%d,%d)", x, y);
                }

#undef CLEARBITS

            }

    } while (done_something);

    /*
     * Solver can make no more progress. See if the grid is full.
     */
    for (i = 0; i < w*h; i++)
	if (!soln[i])
	    return 2;		       /* failed to converge */
    return 1;			       /* success */
}