digit *latin_generate(int o, random_state *rs)
{
    digit *sq;
    int *edges, *backedges, *capacity, *flow;
    void *scratch;
    int ne, scratchsize;
    int i, j, k;
    digit *row, *col, *numinv, *num;

    /*
     * To efficiently generate a latin square in such a way that
     * all possible squares are possible outputs from the function,
     * we make use of a theorem which states that any r x n latin
     * rectangle, with r < n, can be extended into an (r+1) x n
     * latin rectangle. In other words, we can reliably generate a
     * latin square row by row, by at every stage writing down any
     * row at all which doesn't conflict with previous rows, and
     * the theorem guarantees that we will never have to backtrack.
     *
     * To find a viable row at each stage, we can make use of the
     * support functions in maxflow.c.
     */

    sq = snewn(o*o, digit);

    /*
     * In case this method of generation introduces a really subtle
     * top-to-bottom directional bias, we'll generate the rows in
     * random order.
     */
    row = snewn(o, digit);
    col = snewn(o, digit);
    numinv = snewn(o, digit);
    num = snewn(o, digit);
    for (i = 0; i < o; i++)
	row[i] = i;
    shuffle(row, i, sizeof(*row), rs);

    /*
     * Set up the infrastructure for the maxflow algorithm.
     */
    scratchsize = maxflow_scratch_size(o * 2 + 2);
    scratch = smalloc(scratchsize);
    backedges = snewn(o*o + 2*o, int);
    edges = snewn((o*o + 2*o) * 2, int);
    capacity = snewn(o*o + 2*o, int);
    flow = snewn(o*o + 2*o, int);
    /* Set up the edge array, and the initial capacities. */
    ne = 0;
    for (i = 0; i < o; i++) {
	/* Each LHS vertex is connected to all RHS vertices. */
	for (j = 0; j < o; j++) {
	    edges[ne*2] = i;
	    edges[ne*2+1] = j+o;
	    /* capacity for this edge is set later on */
	    ne++;
	}
    }
    for (i = 0; i < o; i++) {
	/* Each RHS vertex is connected to the distinguished sink vertex. */
	edges[ne*2] = i+o;
	edges[ne*2+1] = o*2+1;
	capacity[ne] = 1;
	ne++;
    }
    for (i = 0; i < o; i++) {
	/* And the distinguished source vertex connects to each LHS vertex. */
	edges[ne*2] = o*2;
	edges[ne*2+1] = i;
	capacity[ne] = 1;
	ne++;
    }
    assert(ne == o*o + 2*o);
    /* Now set up backedges. */
    maxflow_setup_backedges(ne, edges, backedges);
    
    /*
     * Now generate each row of the latin square.
     */
    for (i = 0; i < o; i++) {
	/*
	 * To prevent maxflow from behaving deterministically, we
	 * separately permute the columns and the digits for the
	 * purposes of the algorithm, differently for every row.
	 */
	for (j = 0; j < o; j++)
	    col[j] = num[j] = j;
	shuffle(col, j, sizeof(*col), rs);
	shuffle(num, j, sizeof(*num), rs);
	/* We need the num permutation in both forward and inverse forms. */
	for (j = 0; j < o; j++)
	    numinv[num[j]] = j;

	/*
	 * Set up the capacities for the maxflow run, by examining
	 * the existing latin square.
	 */
	for (j = 0; j < o*o; j++)
	    capacity[j] = 1;
	for (j = 0; j < i; j++)
	    for (k = 0; k < o; k++) {
		int n = num[sq[row[j]*o + col[k]] - 1];
		capacity[k*o+n] = 0;
	    }

	/*
	 * Run maxflow.
	 */
	j = maxflow_with_scratch(scratch, o*2+2, 2*o, 2*o+1, ne,
				 edges, backedges, capacity, flow, NULL);
	assert(j == o);   /* by the above theorem, this must have succeeded */

	/*
	 * And examine the flow array to pick out the new row of
	 * the latin square.
	 */
	for (j = 0; j < o; j++) {
	    for (k = 0; k < o; k++) {
		if (flow[j*o+k])
		    break;
	    }
	    assert(k < o);
	    sq[row[i]*o + col[j]] = numinv[k] + 1;
	}
    }

    /*
     * Done. Free our internal workspaces...
     */
    sfree(flow);
    sfree(capacity);
    sfree(edges);
    sfree(backedges);
    sfree(scratch);
    sfree(numinv);
    sfree(num);
    sfree(col);
    sfree(row);

    /*
     * ... and return our completed latin square.
     */
    return sq;
}