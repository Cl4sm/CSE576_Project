static void
setbitmap(Window window)
{
	static XWMHints *hints;
	static Pixmap bitmap_pm;
	static Pixmap mask_pm;
	XWMHints *ohints;

	if (!hints) {
		if (!(hints = XAllocWMHints()) ||
			!(ohints = XAllocWMHints()))
			Fatal_Error("XAllocWMHints");

		if (bitmapname) {
			loadbitmap(window, bitmapname, &bitmap_pm);
			hints->flags |= IconPixmapHint;
			hints->icon_pixmap = bitmap_pm;
		}

		if (maskname) {
			loadbitmap(window, maskname, &mask_pm);
			hints->flags |= IconMaskHint;
			hints->icon_mask = mask_pm;
		}

		XSetCloseDownMode(dpy, RetainTemporary);
	}

	if ((ohints = XGetWMHints(dpy, window)) != NULL ) {
		if (ohints->icon_pixmap && hints->icon_pixmap)
			XFreePixmap(dpy, ohints->icon_pixmap);
		if (ohints->icon_mask && hints->icon_mask)
			XFreePixmap(dpy, ohints->icon_mask);
		XFree(ohints);
	}

	XSetWMHints(dpy, window, hints);
}