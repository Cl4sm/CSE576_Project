static void Destroy(w)
    Widget w;
{
    PixmapWidget PW = (PixmapWidget) w;
    int i;
    
    /* free colorTable */
    for (i = 0; i < 1<<depth; i++)
      if (PW->pixmap.colorTable[i])
	{
	  if (PW->pixmap.colorTable[i]->symbol) 
	    XtFree(PW->pixmap.colorTable[i]->symbol);
	  if (PW->pixmap.colorTable[i]->s_name) 
	    XtFree(PW->pixmap.colorTable[i]->s_name);
	  if (PW->pixmap.colorTable[i]->m_name) 
	    XtFree(PW->pixmap.colorTable[i]->m_name);
	  if (PW->pixmap.colorTable[i]->g4_name) 
	    XtFree(PW->pixmap.colorTable[i]->g4_name);
	  if (PW->pixmap.colorTable[i]->g_name) 
	    XtFree(PW->pixmap.colorTable[i]->g_name);
	  if (PW->pixmap.colorTable[i]->c_name) 
	    XtFree(PW->pixmap.colorTable[i]->c_name);
	  XtFree((char *)PW->pixmap.colorTable[i]);
	}
    XtFree((char *)PW->pixmap.colorTable);

    XFreeGC(dpy, PW->pixmap.drawing_gc);
    XFreeGC(dpy, PW->pixmap.highlighting_gc);
    XFreeGC(dpy, PW->pixmap.framing_gc);
    XFreeGC(dpy, PW->pixmap.transparent_gc);

    DestroyPixmapImage(&PW->pixmap.image);
    DestroyPixmapImage(&PW->pixmap.buffer);
    DestroyMaskImage(&PW->pixmap.mask_image);
    DestroyMaskImage(&PW->pixmap.mask_buffer);
    
    PWRemoveAllRequests(w);
}
