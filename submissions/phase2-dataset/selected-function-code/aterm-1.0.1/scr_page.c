scr_page(int direction, int nlines)
{
    int             start, dirn;

    D_SCREEN((stderr, "scr_page(%s, %d) view_start:%d", ((direction == UP) ? "UP" : "DN"), nlines, TermWin.view_start));

    dirn = (direction == UP) ? 1 : -1;
    start = TermWin.view_start;
    MAX_IT(nlines, 1);
    MIN_IT(nlines, TermWin.nrow);

    if ((dirn == -1 && TermWin.view_start < nlines)) 
	    TermWin.view_start = 0;	
    else
	    TermWin.view_start += (nlines * dirn);
  
    if (dirn == 1 && start > TermWin.view_start)
    	TermWin.view_start = TermWin.nscrolled;    	
		
/*  MAX_IT(TermWin.view_start, 0); */
    MIN_IT(TermWin.view_start, TermWin.nscrolled);

    if (Gr_Displayed())
	Gr_scroll(0);
    return (TermWin.view_start - start);
}
