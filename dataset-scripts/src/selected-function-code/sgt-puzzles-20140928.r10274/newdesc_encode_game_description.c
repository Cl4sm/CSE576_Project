static char *newdesc_encode_game_description(int area, puzzle_size *grid)
{
    char *desc = NULL;
    int desclen = 0, descsize = 0;
    int run, i;

    run = 0;
    for (i = 0; i <= area; i++) {
	int n = (i < area ? grid[i] : -1);

	if (!n)
	    run++;
	else {
	    if (descsize < desclen + 40) {
		descsize = desclen * 3 / 2 + 40;
		desc = sresize(desc, descsize, char);
	    }
	    if (run) {
		while (run > 0) {
		    int c = 'a' - 1 + run;
		    if (run > 26)
			c = 'z';
		    desc[desclen++] = c;
		    run -= c - ('a' - 1);
		}
	    } else {
		/*
		 * If there's a number in the very top left or
		 * bottom right, there's no point putting an
		 * unnecessary _ before or after it.
		 */
		if (desclen > 0 && n > 0)
		    desc[desclen++] = '_';
	    }
	    if (n > 0)
		desclen += sprintf(desc+desclen, "%d", n);
	    run = 0;
	}
    }
    desc[desclen] = '\0';
    return desc;
}