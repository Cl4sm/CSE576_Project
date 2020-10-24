static void ss_add(struct setstore *ss, int x, int y, int mask, int mines)
{
    struct set *s;

    assert(mask != 0);

    /*
     * Normalise so that x and y are genuinely the bounding
     * rectangle.
     */
    while (!(mask & (1|8|64)))
	mask >>= 1, x++;
    while (!(mask & (1|2|4)))
	mask >>= 3, y++;

    /*
     * Create a set structure and add it to the tree.
     */
    s = snew(struct set);
    s->x = x;
    s->y = y;
    s->mask = mask;
    s->mines = mines;
    s->todo = FALSE;
    if (add234(ss->sets, s) != s) {
	/*
	 * This set already existed! Free it and return.
	 */
	sfree(s);
	return;
    }

    /*
     * We've added a new set to the tree, so put it on the todo
     * list.
     */
    ss_add_todo(ss, s);
}