SCM x_list_properties(swin)
     SCM swin;
{
  struct xs_Window *xwn;
  Atom *atoms;
  int num_prop_return;
  SCM lst;
  ASRTER(NIMP(swin) && WINDOWP(swin), swin, ARG1, s_x_map_window);
  xwn = WINDOW(swin);
  atoms = XListProperties(xwn->dpy, xwn->p.win, &num_prop_return);
  {
    int i = num_prop_return;
    lst = EOL;
    while (i--) {
      char *name = XGetAtomName(xwn->dpy, atoms[i]);
      lst = cons(makfrom0str(name), lst);
      XFree(name);
    }
  }
  XFree(atoms);
  return lst;
}
