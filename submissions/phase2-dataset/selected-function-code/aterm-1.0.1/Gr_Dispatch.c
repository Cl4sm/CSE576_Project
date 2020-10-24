void
Gr_Dispatch(grwin_t * grwin, grcmd_t * data)
{
#ifdef RXVT_GRAPHICS
    int             i, n;
    union {
	XPoint          pt[NGRX_PTS / 2];
	XRectangle      rect[NGRX_PTS / 4];
    } xdata;

    if (data->color != Color_fg) {
	XGCValues       gcv;

	gcv.foreground = PixColors[data->color];
	XChangeGC(Xdisplay, TermWin.gc, GCForeground, &gcv);
    }
    if (grwin)
	switch (data->cmd) {
	case 'L':
	    if (data->ncoords > 3) {
		for (n = i = 0; i < data->ncoords; i += 2, n++) {
		    xdata.pt[n].x = data->coords[i] * grwin->w / GRX_SCALE;
		    xdata.pt[n].y = data->coords[i + 1] * grwin->h / GRX_SCALE;
		}
		XDrawLines(Xdisplay,
		       grwin->win, TermWin.gc, xdata.pt, n, CoordModeOrigin);
	    }
	    break;

	case 'P':
	    if (data->ncoords > 3) {
		for (n = i = 0; i < data->ncoords; i += 2, n++) {
		    xdata.pt[n].x = data->coords[i] * grwin->w / GRX_SCALE;
		    xdata.pt[n].y = data->coords[i + 1] * grwin->h / GRX_SCALE;
		}
		XDrawPoints(Xdisplay,
		       grwin->win, TermWin.gc, xdata.pt, n, CoordModeOrigin);
	    }
	    break;

	case 'F':
	    if (data->ncoords > 0) {
		for (n = i = 0; i < data->ncoords; i += 4, n++) {
		    xdata.rect[n].x = data->coords[i] * grwin->w / GRX_SCALE;
		    xdata.rect[n].y = data->coords[i + 1] * grwin->h
				      / GRX_SCALE;
		    xdata.rect[n].width = ((data->coords[i + 2]
					    - data->coords[i] + 1) *
					   grwin->w / GRX_SCALE);
		    xdata.rect[n].height = ((data->coords[i + 3]
					     - data->coords[i + 1] + 1) *
					    grwin->h / GRX_SCALE);
		    XPMClearArea(Xdisplay, grwin->win,
				 xdata.rect[n].x, xdata.rect[n].y,
				 xdata.rect[n].width, xdata.rect[n].height,
				 0);
		}
		XFillRectangles(Xdisplay, grwin->win, TermWin.gc, xdata.rect,
				n);
	    }
	    break;
	case 'T':
	    Gr_Text(grwin, data);
	    break;
	case 'C':
	    Gr_ClearWindow(grwin);
	    break;
	}
    if (data->color != Color_fg) {
	XGCValues       gcv;

	gcv.foreground = PixColors[Color_fg];
	XChangeGC(Xdisplay, TermWin.gc, GCForeground, &gcv);
    }
#endif
}
