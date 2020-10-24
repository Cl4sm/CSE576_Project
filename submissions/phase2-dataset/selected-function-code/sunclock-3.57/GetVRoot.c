     Display *dpy;
{
  int          i;
  Window       rootReturn, parentReturn, *children;
  unsigned int numChildren;
  Atom         __SWM_VROOT = None;
  Window       rslt = Root;

  __SWM_VROOT = XInternAtom(dpy, "__SWM_VROOT", False);
  XQueryTree(dpy, Root, &rootReturn, &parentReturn, &children, &numChildren);
  for (i=0; i<numChildren; i++)
    {
    Atom          actual_type;
    int           actual_format;
    unsigned long nitems, bytesafter;
    Window       *newRoot = NULL;

    /* item 148 in the FAQ neglects to mention that there is a race
     * condition here; consider a child of the root window that
     * existed when XQueryTree() was called, but has disappeared
     * before XGetWindowProperty() gets called for that window ...
     */
    if ((XGetWindowProperty(dpy, children[i], __SWM_VROOT, 0, 1,
                            False, XA_WINDOW, &actual_type,
                            &actual_format, &nitems, &bytesafter,
                            (unsigned char **) &newRoot) == Success)
        && newRoot)
      {
      rslt = *newRoot;
      break;
      }
    }

  /* item 148 in the FAQ also neglects to mention that we probably
   * want to free the list of children after we're done with it ...
   */
  XFree((void *) children);

  return rslt;
}
