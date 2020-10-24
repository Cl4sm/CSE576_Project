static int minesolve(int w, int h, int n, signed char *grid,
		     open_cb open,
                     perturb_cb perturb,
		     void *ctx, random_state *rs)
{
    struct setstore *ss = ss_new();
    struct set **list;
    struct squaretodo astd, *std = &astd;
    int x, y, i, j;
    int nperturbs = 0;

    /*
     * Set up a linked list of squares with known contents, so that
     * we can process them one by one.
     */
    std->next = snewn(w*h, int);
    std->head = std->tail = -1;

    /*
     * Initialise that list with all known squares in the input
     * grid.
     */
    for (y = 0; y < h; y++) {
	for (x = 0; x < w; x++) {
	    i = y*w+x;
	    if (grid[i] != -2)
		std_add(std, i);
	}
    }

    /*
     * Main deductive loop.
     */
    while (1) {
	int done_something = FALSE;
	struct set *s;

	/*
	 * If there are any known squares on the todo list, process
	 * them and construct a set for each.
	 */
	while (std->head != -1) {
	    i = std->head;
#ifdef SOLVER_DIAGNOSTICS
	    printf("known square at %d,%d [%d]\n", i%w, i/w, grid[i]);
#endif
	    std->head = std->next[i];
	    if (std->head == -1)
		std->tail = -1;

	    x = i % w;
	    y = i / w;

	    if (grid[i] >= 0) {
		int dx, dy, mines, bit, val;
#ifdef SOLVER_DIAGNOSTICS
		printf("creating set around this square\n");
#endif
		/*
		 * Empty square. Construct the set of non-known squares
		 * around this one, and determine its mine count.
		 */
		mines = grid[i];
		bit = 1;
		val = 0;
		for (dy = -1; dy <= +1; dy++) {
		    for (dx = -1; dx <= +1; dx++) {
#ifdef SOLVER_DIAGNOSTICS
			printf("grid %d,%d = %d\n", x+dx, y+dy, grid[i+dy*w+dx]);
#endif
			if (x+dx < 0 || x+dx >= w || y+dy < 0 || y+dy >= h)
			    /* ignore this one */;
			else if (grid[i+dy*w+dx] == -1)
			    mines--;
			else if (grid[i+dy*w+dx] == -2)
			    val |= bit;
			bit <<= 1;
		    }
		}
		if (val)
		    ss_add(ss, x-1, y-1, val, mines);
	    }

	    /*
	     * Now, whether the square is empty or full, we must
	     * find any set which contains it and replace it with
	     * one which does not.
	     */
	    {
#ifdef SOLVER_DIAGNOSTICS
		printf("finding sets containing known square %d,%d\n", x, y);
#endif
		list = ss_overlap(ss, x, y, 1);

		for (j = 0; list[j]; j++) {
		    int newmask, newmines;

		    s = list[j];

		    /*
		     * Compute the mask for this set minus the
		     * newly known square.
		     */
		    newmask = setmunge(s->x, s->y, s->mask, x, y, 1, TRUE);

		    /*
		     * Compute the new mine count.
		     */
		    newmines = s->mines - (grid[i] == -1);

		    /*
		     * Insert the new set into the collection,
		     * unless it's been whittled right down to
		     * nothing.
		     */
		    if (newmask)
			ss_add(ss, s->x, s->y, newmask, newmines);

		    /*
		     * Destroy the old one; it is actually obsolete.
		     */
		    ss_remove(ss, s);
		}

		sfree(list);
	    }

	    /*
	     * Marking a fresh square as known certainly counts as
	     * doing something.
	     */
	    done_something = TRUE;
	}

	/*
	 * Now pick a set off the to-do list and attempt deductions
	 * based on it.
	 */
	if ((s = ss_todo(ss)) != NULL) {

#ifdef SOLVER_DIAGNOSTICS
	    printf("set to do: %d,%d %03x %d\n", s->x, s->y, s->mask, s->mines);
#endif
	    /*
	     * Firstly, see if this set has a mine count of zero or
	     * of its own cardinality.
	     */
	    if (s->mines == 0 || s->mines == bitcount16(s->mask)) {
		/*
		 * If so, we can immediately mark all the squares
		 * in the set as known.
		 */
#ifdef SOLVER_DIAGNOSTICS
		printf("easy\n");
#endif
		known_squares(w, h, std, grid, open, ctx,
			      s->x, s->y, s->mask, (s->mines != 0));

		/*
		 * Having done that, we need do nothing further
		 * with this set; marking all the squares in it as
		 * known will eventually eliminate it, and will
		 * also permit further deductions about anything
		 * that overlaps it.
		 */
		continue;
	    }

	    /*
	     * Failing that, we now search through all the sets
	     * which overlap this one.
	     */
	    list = ss_overlap(ss, s->x, s->y, s->mask);

	    for (j = 0; list[j]; j++) {
		struct set *s2 = list[j];
		int swing, s2wing, swc, s2wc;

		/*
		 * Find the non-overlapping parts s2-s and s-s2,
		 * and their cardinalities.
		 * 
		 * I'm going to refer to these parts as `wings'
		 * surrounding the central part common to both
		 * sets. The `s wing' is s-s2; the `s2 wing' is
		 * s2-s.
		 */
		swing = setmunge(s->x, s->y, s->mask, s2->x, s2->y, s2->mask,
				 TRUE);
		s2wing = setmunge(s2->x, s2->y, s2->mask, s->x, s->y, s->mask,
				 TRUE);
		swc = bitcount16(swing);
		s2wc = bitcount16(s2wing);

		/*
		 * If one set has more mines than the other, and
		 * the number of extra mines is equal to the
		 * cardinality of that set's wing, then we can mark
		 * every square in the wing as a known mine, and
		 * every square in the other wing as known clear.
		 */
		if (swc == s->mines - s2->mines ||
		    s2wc == s2->mines - s->mines) {
		    known_squares(w, h, std, grid, open, ctx,
				  s->x, s->y, swing,
				  (swc == s->mines - s2->mines));
		    known_squares(w, h, std, grid, open, ctx,
				  s2->x, s2->y, s2wing,
				  (s2wc == s2->mines - s->mines));
		    continue;
		}

		/*
		 * Failing that, see if one set is a subset of the
		 * other. If so, we can divide up the mine count of
		 * the larger set between the smaller set and its
		 * complement, even if neither smaller set ends up
		 * being immediately clearable.
		 */
		if (swc == 0 && s2wc != 0) {
		    /* s is a subset of s2. */
		    assert(s2->mines > s->mines);
		    ss_add(ss, s2->x, s2->y, s2wing, s2->mines - s->mines);
		} else if (s2wc == 0 && swc != 0) {
		    /* s2 is a subset of s. */
		    assert(s->mines > s2->mines);
		    ss_add(ss, s->x, s->y, swing, s->mines - s2->mines);
		}
	    }

	    sfree(list);

	    /*
	     * In this situation we have definitely done
	     * _something_, even if it's only reducing the size of
	     * our to-do list.
	     */
	    done_something = TRUE;
	} else if (n >= 0) {
	    /*
	     * We have nothing left on our todo list, which means
	     * all localised deductions have failed. Our next step
	     * is to resort to global deduction based on the total
	     * mine count. This is computationally expensive
	     * compared to any of the above deductions, which is
	     * why we only ever do it when all else fails, so that
	     * hopefully it won't have to happen too often.
	     * 
	     * If you pass n<0 into this solver, that informs it
	     * that you do not know the total mine count, so it
	     * won't even attempt these deductions.
	     */

	    int minesleft, squaresleft;
	    int nsets, setused[10], cursor;

	    /*
	     * Start by scanning the current grid state to work out
	     * how many unknown squares we still have, and how many
	     * mines are to be placed in them.
	     */
	    squaresleft = 0;
	    minesleft = n;
	    for (i = 0; i < w*h; i++) {
		if (grid[i] == -1)
		    minesleft--;
		else if (grid[i] == -2)
		    squaresleft++;
	    }

#ifdef SOLVER_DIAGNOSTICS
	    printf("global deduction time: squaresleft=%d minesleft=%d\n",
		   squaresleft, minesleft);
	    for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
		    int v = grid[y*w+x];
		    if (v == -1)
			putchar('*');
		    else if (v == -2)
			putchar('?');
		    else if (v == 0)
			putchar('-');
		    else
			putchar('0' + v);
		}
		putchar('\n');
	    }
#endif

	    /*
	     * If there _are_ no unknown squares, we have actually
	     * finished.
	     */
	    if (squaresleft == 0) {
		assert(minesleft == 0);
		break;
	    }

	    /*
	     * First really simple case: if there are no more mines
	     * left, or if there are exactly as many mines left as
	     * squares to play them in, then it's all easy.
	     */
	    if (minesleft == 0 || minesleft == squaresleft) {
		for (i = 0; i < w*h; i++)
		    if (grid[i] == -2)
			known_squares(w, h, std, grid, open, ctx,
				      i % w, i / w, 1, minesleft != 0);
		continue;	       /* now go back to main deductive loop */
	    }

	    /*
	     * Failing that, we have to do some _real_ work.
	     * Ideally what we do here is to try every single
	     * combination of the currently available sets, in an
	     * attempt to find a disjoint union (i.e. a set of
	     * squares with a known mine count between them) such
	     * that the remaining unknown squares _not_ contained
	     * in that union either contain no mines or are all
	     * mines.
	     * 
	     * Actually enumerating all 2^n possibilities will get
	     * a bit slow for large n, so I artificially cap this
	     * recursion at n=10 to avoid too much pain.
	     */
	    nsets = count234(ss->sets);
	    if (nsets <= lenof(setused)) {
		/*
		 * Doing this with actual recursive function calls
		 * would get fiddly because a load of local
		 * variables from this function would have to be
		 * passed down through the recursion. So instead
		 * I'm going to use a virtual recursion within this
		 * function. The way this works is:
		 * 
		 *  - we have an array `setused', such that
		 *    setused[n] is 0 or 1 depending on whether set
		 *    n is currently in the union we are
		 *    considering.
		 * 
		 *  - we have a value `cursor' which indicates how
		 *    much of `setused' we have so far filled in.
		 *    It's conceptually the recursion depth.
		 * 
		 * We begin by setting `cursor' to zero. Then:
		 * 
		 *  - if cursor can advance, we advance it by one.
		 *    We set the value in `setused' that it went
		 *    past to 1 if that set is disjoint from
		 *    anything else currently in `setused', or to 0
		 *    otherwise.
		 * 
		 *  - If cursor cannot advance because it has
		 *    reached the end of the setused list, then we
		 *    have a maximal disjoint union. Check to see
		 *    whether its mine count has any useful
		 *    properties. If so, mark all the squares not
		 *    in the union as known and terminate.
		 * 
		 *  - If cursor has reached the end of setused and
		 *    the algorithm _hasn't_ terminated, back
		 *    cursor up to the nearest 1, turn it into a 0
		 *    and advance cursor just past it.
		 * 
		 *  - If we attempt to back up to the nearest 1 and
		 *    there isn't one at all, then we have gone
		 *    through all disjoint unions of sets in the
		 *    list and none of them has been helpful, so we
		 *    give up.
		 */
		struct set *sets[lenof(setused)];
		for (i = 0; i < nsets; i++)
		    sets[i] = index234(ss->sets, i);

		cursor = 0;
		while (1) {

		    if (cursor < nsets) {
			int ok = TRUE;

			/* See if any existing set overlaps this one. */
			for (i = 0; i < cursor; i++)
			    if (setused[i] &&
				setmunge(sets[cursor]->x,
					 sets[cursor]->y,
					 sets[cursor]->mask,
					 sets[i]->x, sets[i]->y, sets[i]->mask,
					 FALSE)) {
				ok = FALSE;
				break;
			    }

			if (ok) {
			    /*
			     * We're adding this set to our union,
			     * so adjust minesleft and squaresleft
			     * appropriately.
			     */
			    minesleft -= sets[cursor]->mines;
			    squaresleft -= bitcount16(sets[cursor]->mask);
			}

			setused[cursor++] = ok;
		    } else {
#ifdef SOLVER_DIAGNOSTICS
			printf("trying a set combination with %d %d\n",
			       squaresleft, minesleft);
#endif /* SOLVER_DIAGNOSTICS */

			/*
			 * We've reached the end. See if we've got
			 * anything interesting.
			 */
			if (squaresleft > 0 &&
			    (minesleft == 0 || minesleft == squaresleft)) {
			    /*
			     * We have! There is at least one
			     * square not contained within the set
			     * union we've just found, and we can
			     * deduce that either all such squares
			     * are mines or all are not (depending
			     * on whether minesleft==0). So now all
			     * we have to do is actually go through
			     * the grid, find those squares, and
			     * mark them.
			     */
			    for (i = 0; i < w*h; i++)
				if (grid[i] == -2) {
				    int outside = TRUE;
				    y = i / w;
				    x = i % w;
				    for (j = 0; j < nsets; j++)
					if (setused[j] &&
					    setmunge(sets[j]->x, sets[j]->y,
						     sets[j]->mask, x, y, 1,
						     FALSE)) {
					    outside = FALSE;
					    break;
					}
				    if (outside)
					known_squares(w, h, std, grid,
						      open, ctx,
						      x, y, 1, minesleft != 0);
				}

			    done_something = TRUE;
			    break;     /* return to main deductive loop */
			}

			/*
			 * If we reach here, then this union hasn't
			 * done us any good, so move on to the
			 * next. Backtrack cursor to the nearest 1,
			 * change it to a 0 and continue.
			 */
			while (--cursor >= 0 && !setused[cursor]);
			if (cursor >= 0) {
			    assert(setused[cursor]);

			    /*
			     * We're removing this set from our
			     * union, so re-increment minesleft and
			     * squaresleft.
			     */
			    minesleft += sets[cursor]->mines;
			    squaresleft += bitcount16(sets[cursor]->mask);

			    setused[cursor++] = 0;
			} else {
			    /*
			     * We've backtracked all the way to the
			     * start without finding a single 1,
			     * which means that our virtual
			     * recursion is complete and nothing
			     * helped.
			     */
			    break;
			}
		    }

		}

	    }
	}

	if (done_something)
	    continue;

#ifdef SOLVER_DIAGNOSTICS
	/*
	 * Dump the current known state of the grid.
	 */
	printf("solver ran out of steam, ret=%d, grid:\n", nperturbs);
	for (y = 0; y < h; y++) {
	    for (x = 0; x < w; x++) {
		int v = grid[y*w+x];
		if (v == -1)
		    putchar('*');
		else if (v == -2)
		    putchar('?');
		else if (v == 0)
		    putchar('-');
		else
		    putchar('0' + v);
	    }
	    putchar('\n');
	}

	{
	    struct set *s;

	    for (i = 0; (s = index234(ss->sets, i)) != NULL; i++)
		printf("remaining set: %d,%d %03x %d\n", s->x, s->y, s->mask, s->mines);
	}
#endif

	/*
	 * Now we really are at our wits' end as far as solving
	 * this grid goes. Our only remaining option is to call
	 * a perturb function and ask it to modify the grid to
	 * make it easier.
	 */
	if (perturb) {
	    struct perturbations *ret;
	    struct set *s;

	    nperturbs++;

	    /*
	     * Choose a set at random from the current selection,
	     * and ask the perturb function to either fill or empty
	     * it.
	     * 
	     * If we have no sets at all, we must give up.
	     */
	    if (count234(ss->sets) == 0) {
#ifdef SOLVER_DIAGNOSTICS
		printf("perturbing on entire unknown set\n");
#endif
		ret = perturb(ctx, grid, 0, 0, 0);
	    } else {
		s = index234(ss->sets, random_upto(rs, count234(ss->sets)));
#ifdef SOLVER_DIAGNOSTICS
		printf("perturbing on set %d,%d %03x\n", s->x, s->y, s->mask);
#endif
		ret = perturb(ctx, grid, s->x, s->y, s->mask);
	    }

	    if (ret) {
		assert(ret->n > 0);    /* otherwise should have been NULL */

		/*
		 * A number of squares have been fiddled with, and
		 * the returned structure tells us which. Adjust
		 * the mine count in any set which overlaps one of
		 * those squares, and put them back on the to-do
		 * list. Also, if the square itself is marked as a
		 * known non-mine, put it back on the squares-to-do
		 * list.
		 */
		for (i = 0; i < ret->n; i++) {
#ifdef SOLVER_DIAGNOSTICS
		    printf("perturbation %s mine at %d,%d\n",
			   ret->changes[i].delta > 0 ? "added" : "removed",
			   ret->changes[i].x, ret->changes[i].y);
#endif

		    if (ret->changes[i].delta < 0 &&
			grid[ret->changes[i].y*w+ret->changes[i].x] != -2) {
			std_add(std, ret->changes[i].y*w+ret->changes[i].x);
		    }

		    list = ss_overlap(ss,
				      ret->changes[i].x, ret->changes[i].y, 1);

		    for (j = 0; list[j]; j++) {
			list[j]->mines += ret->changes[i].delta;
			ss_add_todo(ss, list[j]);
		    }

		    sfree(list);
		}

		/*
		 * Now free the returned data.
		 */
		sfree(ret->changes);
		sfree(ret);

#ifdef SOLVER_DIAGNOSTICS
		/*
		 * Dump the current known state of the grid.
		 */
		printf("state after perturbation:\n");
		for (y = 0; y < h; y++) {
		    for (x = 0; x < w; x++) {
			int v = grid[y*w+x];
			if (v == -1)
			    putchar('*');
			else if (v == -2)
			    putchar('?');
			else if (v == 0)
			    putchar('-');
			else
			    putchar('0' + v);
		    }
		    putchar('\n');
		}

		{
		    struct set *s;

		    for (i = 0; (s = index234(ss->sets, i)) != NULL; i++)
			printf("remaining set: %d,%d %03x %d\n", s->x, s->y, s->mask, s->mines);
		}
#endif

		/*
		 * And now we can go back round the deductive loop.
		 */
		continue;
	    }
	}

	/*
	 * If we get here, even that didn't work (either we didn't
	 * have a perturb function or it returned failure), so we
	 * give up entirely.
	 */
	break;
    }

    /*
     * See if we've got any unknown squares left.
     */
    for (y = 0; y < h; y++)
	for (x = 0; x < w; x++)
	    if (grid[y*w+x] == -2) {
		nperturbs = -1;	       /* failed to complete */
		break;
	    }

    /*
     * Free the set list and square-todo list.
     */
    {
	struct set *s;
	while ((s = delpos234(ss->sets, 0)) != NULL)
	    sfree(s);
	freetree234(ss->sets);
	sfree(ss);
	sfree(std->next);
    }

    return nperturbs;
}