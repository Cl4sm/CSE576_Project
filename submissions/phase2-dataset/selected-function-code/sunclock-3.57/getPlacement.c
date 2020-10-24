getPlacement(win, x, y, w, h)
Window win;
int *x, *y;
unsigned int *w, *h;
{
   int xp, yp;
   unsigned int b, d, n;
   Window junk, root, parent, *children;

   XFlush(dpy);
   XQueryTree(dpy, win, &root, &parent, &children, &n);
   if (!parent) {
      fprintf(stderr, "Cannot query window tree!\n");
      return 1;
   }
   
   if (parent == root) parent = win;

   XGetGeometry(dpy, parent, &root, x, y, w, h, &b, &d);

   XTranslateCoordinates(dpy, win, parent, 0, 0, x, y, &junk);
   if (2*(*x) < *w) *w -= 2*(*x);
   if ((*x)+(*y) < *h) *h -= (*x) + (*y);
   XTranslateCoordinates(dpy, win, root, 0, 0, x, y, &junk);
   XTranslateCoordinates(dpy, parent, root, 0, 0, &xp, &yp, &junk);

   horiz_drift = *x - xp;
   vert_drift = *y - yp;
/*
   if (verbose) {
      fprintf(stderr, "Window placement (%d,%d)\n", *x, *y);	
      fprintf(stderr, "Window drift (%d,%d)\n", horiz_drift, vert_drift);
   }
*/
   return 0;
}
