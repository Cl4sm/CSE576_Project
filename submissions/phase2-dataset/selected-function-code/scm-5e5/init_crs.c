{
  /*  savetty(); */
  /* "Stdscr" is a nearly inaccessible symbol used as a GC protect. */
  loc_stdscr = &CDR(sysintern("Stdscr", UNDEFINED));
  tc16_window = newptob(&winptob);

  init_iprocs(subr0s, tc7_subr_0);
  init_iprocs(subr1s, tc7_subr_1);
  init_iprocs(subr2s, tc7_subr_2);

  make_subr(s_owidth, tc7_subr_1o, owidth);
  make_subr(s_oheight, tc7_subr_1o, oheight);

  make_subr(s_newwin, tc7_lsubr_2, lnewwin);
  make_subr(s_subwin, tc7_lsubr_2, lsubwin);

  make_subr(s_wmove, tc7_subr_3, lwmove);
  make_subr(s_mvwin, tc7_subr_3, lmvwin);
  make_subr(s_box, tc7_subr_3, lbox);
  add_feature("curses");
  add_final(lendwin);
}
