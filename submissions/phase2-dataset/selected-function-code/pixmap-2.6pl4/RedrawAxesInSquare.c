     PixmapWidget PW;
     Position x, y;
{
  int w = PW->pixmap.width, h =PW->pixmap.height;
  XPoint P1, Q1, P2, Q2, *A1, *A2, *B1, *B2, *from, *to;

  if (PWQueryAxes((Widget)PW))
    {
      /* first diagonal */
      P2.x = InWindowX(PW, 0); P2.y = InWindowY(PW, 0);
      Q2.x = InWindowX(PW, w); Q2.y = InWindowY(PW, h);
      
      /* A1: top intersect diagonal */
      P1.x = InWindowX(PW, x); P1.y = InWindowY(PW, y);
      Q1.x = InWindowX(PW, x+1); Q1.y = P1.y;
      A1 = intersect(P1, Q1, P2, Q2);
      
      /* A2: left intersect diagonal */
      P1.x = InWindowX(PW, x); P1.y = InWindowY(PW, y);
      Q1.x = P1.x; Q1.y = InWindowY(PW, y+1);
      A2 = intersect(P1, Q1, P2, Q2);
      
      /* B1: bottom intersect diagonal */
      P1.x = InWindowX(PW, x); P1.y = InWindowY(PW, y+1);
      Q1.x = InWindowX(PW, x+1); Q1.y = P1.y;
      B1 = intersect(P1, Q1, P2, Q2);
      
      /* B2: right intersect diagonal */
      P1.x = InWindowX(PW, x+1); P1.y = InWindowY(PW, y);
      Q1.x = P1.x; Q1.y = InWindowY(PW, y+1);
      B2 = intersect(P1, Q1, P2, Q2);
      
      /* line to draw from=OR(a1, a2) to=OR(b1, b2) */
      from = OR(A1, A2); to = OR(B1, B2);
      if (from && to) XDrawLine(dpy, XtWindow(PW), PW->pixmap.framing_gc,
				from->x, from->y, to->x, to->y);
      XtFree((char *)A1); XtFree((char *)A2);
      XtFree((char *)B1); XtFree((char *)B2);
      
      /* second diagonal */
      P2.x = InWindowX(PW, 0); P2.y = InWindowY(PW, h);
      Q2.x = InWindowX(PW, w); Q2.y = InWindowY(PW, 0);
      
      /* A1: top intersect diagonal */
      P1.x = InWindowX(PW, x); P1.y = InWindowY(PW, y);
      Q1.x = InWindowX(PW, x+1); Q1.y = P1.y;
      A1 = intersect(P1, Q1, P2, Q2);
      
      /* A2: right intersect diagonal */
      P1.x = InWindowX(PW, x+1); P1.y = InWindowY(PW, y);
      Q1.x = P1.x; Q1.y = InWindowY(PW, y+1);
      A2 = intersect(P1, Q1, P2, Q2);
      
      /* B1: bottom intersect diagonal */
      P1.x = InWindowX(PW, x); P1.y = InWindowY(PW, y+1);
      Q1.x = InWindowX(PW, x+1); Q1.y = P1.y;
      B1 = intersect(P1, Q1, P2, Q2);
      
      /* B2: left intersect diagonal */
      P1.x = InWindowX(PW, x); P1.y = InWindowY(PW, y);
      Q1.x = P1.x; Q1.y = InWindowY(PW, y+1);
      B2 = intersect(P1, Q1, P2, Q2);
      
      /* line to draw from=OR(a1, a2) to=OR(b1, b2) */
      from = OR(A1, A2); to = OR(B1, B2);
      if (from && to) XDrawLine(dpy, XtWindow(PW), PW->pixmap.framing_gc,
				from->x, from->y, to->x, to->y);
      XtFree((char *)A1); XtFree((char *)A2);
      XtFree((char *)B1); XtFree((char *)B2);

      /* horizontal */
      if (((h % 2) != 0) || (!PWQueryGrid((Widget)PW))) /* height is even 
                                                           or grid is off */
	{
	  Position y0;
	  
	  y0 = InWindowY(PW, 0)
	    + roundint((InWindowY(PW, h) - InWindowY(PW, 0)) / 2.);
	  if ((y0 > InWindowY(PW, y)) && (y0 < InWindowY(PW, y+1)))
	    XDrawLine(dpy, XtWindow(PW), PW->pixmap.framing_gc,
		      InWindowX(PW, x), y0, InWindowX(PW, x+1), y0);
	}
      
      /* vertical */
      if (((w % 2) != 0) || (!PWQueryGrid((Widget)PW))) /* width is even 
                                                           or grid is off */
	{
	  Position x0;
	  
	  x0 = InWindowX(PW, 0)
	    + roundint((InWindowX(PW, w) - InWindowX(PW, 0)) / 2.);
	  if ((x0 > InWindowX(PW, x)) && (x0 < InWindowX(PW, x+1)))
	    XDrawLine(dpy, XtWindow(PW), PW->pixmap.framing_gc,
		      x0, InWindowY(PW, y), x0, InWindowY(PW, y+1));
	}
    }
}
