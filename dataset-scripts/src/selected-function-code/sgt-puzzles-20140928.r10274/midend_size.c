void midend_size(midend *me, int *x, int *y, int user_size)
{
    int min, max;
    int rx, ry;

    /*
     * We can't set the size on the same drawstate twice. So if
     * we've already sized one drawstate, we must throw it away and
     * create a new one.
     */
    if (me->drawstate && me->tilesize > 0) {
        me->ourgame->free_drawstate(me->drawing, me->drawstate);
        me->drawstate = me->ourgame->new_drawstate(me->drawing,
                                                   me->states[0].state);
    }

    /*
     * Find the tile size that best fits within the given space. If
     * `user_size' is TRUE, we must actually find the _largest_ such
     * tile size, in order to get as close to the user's explicit
     * request as possible; otherwise, we bound above at the game's
     * preferred tile size, so that the game gets what it wants
     * provided that this doesn't break the constraint from the
     * front-end (which is likely to be a screen size or similar).
     */
    if (user_size) {
	max = 1;
	do {
	    max *= 2;
	    me->ourgame->compute_size(me->params, max, &rx, &ry);
	} while (rx <= *x && ry <= *y);
    } else
	max = me->preferred_tilesize + 1;
    min = 1;

    /*
     * Now binary-search between min and max. We're looking for a
     * boundary rather than a value: the point at which tile sizes
     * stop fitting within the given dimensions. Thus, we stop when
     * max and min differ by exactly 1.
     */
    while (max - min > 1) {
	int mid = (max + min) / 2;
	me->ourgame->compute_size(me->params, mid, &rx, &ry);
	if (rx <= *x && ry <= *y)
	    min = mid;
	else
	    max = mid;
    }

    /*
     * Now `min' is a valid size, and `max' isn't. So use `min'.
     */

    me->tilesize = min;
    if (user_size)
        /* If the user requested a change in size, make it permanent. */
        me->preferred_tilesize = me->tilesize;
    midend_size_new_drawstate(me);
    *x = me->winwidth;
    *y = me->winheight;
}