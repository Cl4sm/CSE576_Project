int
rxvt_privcases(rxvt_t* r, int page, int mode, uint32_t bit)
{
    int		 state;

    if (mode == 's')
    {
	SET_PMODE (r, page, ISSET_PMODE(r, page, bit));
	return -1;
    }
    else
    {
	if (mode == 'r')
	    /* no overlapping */
	    state = ISSET_SMODE(r, page, bit) ? 1 : 0;
	else
	    state = (mode == 't') ? !ISSET_PMODE(r, page, bit) : mode;
	PrivMode(state, bit, page);
    }
    return state;
}