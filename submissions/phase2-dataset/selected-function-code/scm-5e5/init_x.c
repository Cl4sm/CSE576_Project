void init_x()
{
  init_iprocs(x_subr3s, tc7_subr_3);
  init_iprocs(x_lsubr2s, tc7_lsubr_2);
  init_iprocs(x_lsubrs, tc7_lsubr);
  init_iprocs(x_subr2s, tc7_subr_2);
  init_iprocs(x_subr2os, tc7_subr_2o);
  init_iprocs(x_subr1s, tc7_subr_1);

  tc16_xdisplay = newsmob(&smob_xdisplay);
  tc16_xwindow = newsmob(&smob_xwindow);
  tc16_xcursor = newsmob(&smob_xcursor);
  tc16_xfont = newsmob(&smob_xfont);
  tc16_xcolormap = newsmob(&smob_xcolormap);
  tc16_xgcontext = newsmob(&smob_xgcontext);
  tc16_xvisual = newsmob(&smob_xvisual);
  tc16_xevent = newsmob(&smob_xevent);
  tc16_xccc = newsmob(&smob_xccc);
  xtc_ccc = XUniqueContext();
  xtc_cmp = XUniqueContext();

  scm_ldprog("x11.scm");
  scm_ldprog("xevent.scm");
				/* Redefines STRING */
/*    scm_ldprog("xatoms.scm"); */
  scm_ldstr("\
(define x:ccc x:default-ccc)\n\
(define x:GC-Clip-Origin (logior x:GC-Clip-X-Origin x:GC-Clip-Y-Origin))\n\
(define x:GC-Tile-Stip-Origin \n\
        (logior x:GC-Tile-Stip-X-Origin x:GC-Tile-Stip-Y-Origin))\n\
");
  add_feature("xlib");

  add_final(x_scm_final);
  XSetErrorHandler(x_scm_error_handler);
}
