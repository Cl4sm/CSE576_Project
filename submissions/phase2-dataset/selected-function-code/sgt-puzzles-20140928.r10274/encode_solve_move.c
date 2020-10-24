static char *encode_solve_move(const game_state *state)
{
    int len;
    char *ret, *p;
    int i;
    int num_edges = state->game_grid->num_edges;

    /* This is going to return a string representing the moves needed to set
     * every line in a grid to be the same as the ones in 'state'.  The exact
     * length of this string is predictable. */

    len = 1;  /* Count the 'S' prefix */
    /* Numbers in all lines */
    len += len_0_to_n(num_edges);
    /* For each line we also have a letter */
    len += num_edges;

    ret = snewn(len + 1, char);
    p = ret;

    p += sprintf(p, "S");

    for (i = 0; i < num_edges; i++) {
        switch (state->lines[i]) {
	  case LINE_YES:
	    p += sprintf(p, "%dy", i);
	    break;
	  case LINE_NO:
	    p += sprintf(p, "%dn", i);
	    break;
        }
    }

    /* No point in doing sums like that if they're going to be wrong */
    assert(strlen(ret) <= (size_t)len);
    return ret;
}