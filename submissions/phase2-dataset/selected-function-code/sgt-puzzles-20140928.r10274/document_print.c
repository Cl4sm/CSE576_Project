void document_print(document *doc, drawing *dr)
{
    int ppp;			       /* puzzles per page */
    int pages, passes;
    int page, pass;
    int pageno;

    ppp = doc->pw * doc->ph;
    pages = (doc->npuzzles + ppp - 1) / ppp;
    passes = (doc->got_solns ? 2 : 1);

    print_begin_doc(dr, pages * passes);

    pageno = 1;
    for (pass = 0; pass < passes; pass++) {
	for (page = 0; page < pages; page++) {
	    int i, n, offset;
	    float colsum, rowsum;

	    print_begin_page(dr, pageno);

	    offset = page * ppp;
	    n = min(ppp, doc->npuzzles - offset);

	    for (i = 0; i < doc->pw; i++)
		doc->colwid[i] = 0;
	    for (i = 0; i < doc->ph; i++)
		doc->rowht[i] = 0;

	    /*
	     * Lay the page out by computing all the puzzle sizes.
	     */
	    for (i = 0; i < n; i++) {
		struct puzzle *pz = doc->puzzles + offset + i;
		int x = i % doc->pw, y = i / doc->pw;
		float w, h, scale;

		get_puzzle_size(doc, pz, &w, &h, &scale);

		/* Update the maximum width/height of this column. */
		doc->colwid[x] = max(doc->colwid[x], w);
		doc->rowht[y] = max(doc->rowht[y], h);
	    }

	    /*
	     * Add up the maximum column/row widths to get the
	     * total amount of space used up by puzzles on the
	     * page. We will use this to compute gutter widths.
	     */
	    colsum = 0.0;
	    for (i = 0; i < doc->pw; i++)
		colsum += doc->colwid[i];
	    rowsum = 0.0;
	    for (i = 0; i < doc->ph; i++)
		rowsum += doc->rowht[i];

	    /*
	     * Now do the printing.
	     */
	    for (i = 0; i < n; i++) {
		struct puzzle *pz = doc->puzzles + offset + i;
		int x = i % doc->pw, y = i / doc->pw, j;
		float w, h, scale, xm, xc, ym, yc;
		int pixw, pixh, tilesize;

		if (pass == 1 && !pz->st2)
		    continue;	       /* nothing to do */

		/*
		 * The total amount of gutter space is the page
		 * width minus colsum. This is divided into pw+1
		 * gutters, so the amount of horizontal gutter
		 * space appearing to the left of this puzzle
		 * column is
		 * 
		 *   (width-colsum) * (x+1)/(pw+1)
		 * = width * (x+1)/(pw+1) - (colsum * (x+1)/(pw+1))
		 */
		xm = (float)(x+1) / (doc->pw + 1);
		xc = -xm * colsum;
		/* And similarly for y. */
		ym = (float)(y+1) / (doc->ph + 1);
		yc = -ym * rowsum;

		/*
		 * However, the amount of space to the left of this
		 * puzzle isn't just gutter space: we must also
		 * count the widths of all the previous columns.
		 */
		for (j = 0; j < x; j++)
		    xc += doc->colwid[j];
		/* And similarly for rows. */
		for (j = 0; j < y; j++)
		    yc += doc->rowht[j];

		/*
		 * Now we adjust for this _specific_ puzzle, which
		 * means centring it within the cell we've just
		 * computed.
		 */
		get_puzzle_size(doc, pz, &w, &h, &scale);
		xc += (doc->colwid[x] - w) / 2;
		yc += (doc->rowht[y] - h) / 2;

		/*
		 * And now we know where and how big we want to
		 * print the puzzle, just go ahead and do so. For
		 * the moment I'll pick a standard pixel tile size
		 * of 512.
		 * 
		 * (FIXME: would it be better to pick this value
		 * with reference to the printer resolution? Or
		 * permit each game to choose its own?)
		 */
		tilesize = 512;
		pz->game->compute_size(pz->par, tilesize, &pixw, &pixh);
		print_begin_puzzle(dr, xm, xc, ym, yc, pixw, pixh, w, scale);
		pz->game->print(dr, pass == 0 ? pz->st : pz->st2, tilesize);
		print_end_puzzle(dr);
	    }

	    print_end_page(dr, pageno);
	    pageno++;
	}
    }

    print_end_doc(dr);
}