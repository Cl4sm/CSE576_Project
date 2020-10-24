static char *game_text_format(const game_state *state)
{
    char *ret, *p, buf[80];
    int x, y, col, maxlen;

    /*
     * First work out how many characters we need to display each
     * number.
     */
    col = sprintf(buf, "%d", state->n-1);

    /*
     * Now we know the exact total size of the grid we're going to
     * produce: it's got h rows, each containing w lots of col, w-1
     * spaces and a trailing newline.
     */
    maxlen = state->h * state->w * (col+1);

    ret = snewn(maxlen+1, char);
    p = ret;

    for (y = 0; y < state->h; y++) {
	for (x = 0; x < state->w; x++) {
	    int v = state->tiles[state->w*y+x];
	    if (v == 0)
		sprintf(buf, "%*s", col, "");
	    else
		sprintf(buf, "%*d", col, v);
	    memcpy(p, buf, col);
	    p += col;
	    if (x+1 == state->w)
		*p++ = '\n';
	    else
		*p++ = ' ';
	}
    }

    assert(p - ret == maxlen);
    *p = '\0';
    return ret;
}