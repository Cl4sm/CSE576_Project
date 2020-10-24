 * Paste a selection direct to the command fd
 */
/* INTPROTO */
void
rxvt_paste_str(rxvt_t* r, int page,
	const unsigned char *data, unsigned int nitems)
{
    unsigned int    i, j, n;
    unsigned char  *ds = rxvt_malloc(PROP_SIZE);
    
    /*
     * Convert normal newline chars into common keyboard Return key sequence
     */
    for (i = 0; i < nitems; i += PROP_SIZE)
    {
	n = min(nitems - i, PROP_SIZE);
	MEMCPY(ds, data + i, n);
	for (j = 0; j < n; j++)
	    if (ds[j] == '\n')
		ds[j] = '\r';
	rxvt_tt_write(r, page, ds, (int)n);