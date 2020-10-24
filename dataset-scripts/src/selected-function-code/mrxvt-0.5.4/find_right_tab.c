static int
find_right_tab (rxvt_t* r, int start, int distance)
{
    register int    i, left;

    /* Sanatization */
    if (LTAB(r) == start)
	return start;

    /* BUG: tab overlap with button */
    if (distance < 0)
	return start;

    left = distance;
    for (i = start + 1; i <= LTAB(r); i ++)
    {
	if (left < TAB_WIDTH(i))
	    break;
	left -= (TAB_WIDTH(i));
    }
    return (i - 1);
}