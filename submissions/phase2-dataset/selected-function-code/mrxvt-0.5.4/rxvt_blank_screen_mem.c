void
rxvt_blank_screen_mem(rxvt_t* r, int page, text_t **tp, rend_t **rp,
	unsigned int row, rend_t efs)
{
    int		 width = r->TermWin.ncol;
    rend_t	 *er;

    assert ((tp[row] && rp[row]) ||
	(tp[row] == NULL && rp[row] == NULL));

    /* possible integer overflow? */
    assert (width > 0);
    assert (sizeof (text_t) * width > 0);
    assert (sizeof (rend_t) * width > 0);

    if (tp[row] == NULL)
    {
	tp[row] = rxvt_malloc(sizeof(text_t) * width);
	rp[row] = rxvt_malloc(sizeof(rend_t) * width);
    }
    MEMSET(tp[row], ' ', width);
    efs &= ~RS_baseattrMask;
    for (er = rp[row]; width--;)
	*er++ = efs;
}