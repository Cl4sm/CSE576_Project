static int
find_left_tab (rxvt_t* r, int start, int distance)
{
    register int    i, left;

    /* Sanatization */
    if (0 == start)
	return 0;

    /* BUG: tab overlap with button */
    if (distance < 0)
	return start;

    left = distance;
    for (i = start - 1; i >= 0; i --)
    {
	if (left < TAB_WIDTH(i))
	    break;
	left -= (TAB_WIDTH(i));
    }
    return (i + 1);
}