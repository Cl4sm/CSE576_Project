void scm_brk_report()
{
  unsigned long scm_curbrk = (unsigned long)sbrk(0),
    dif1 = ((dumped ? scm_dumped_brk : scm_curbrk) - scm_init_brk)/1024,
    dif2 = (scm_curbrk - scm_dumped_brk)/1024;

  lputs("initial brk = 0x", cur_errp);
  scm_intprint(scm_init_brk, -16, cur_errp);
  if (dumped) {
    lputs(", dumped = 0x", cur_errp);
    scm_intprint(scm_dumped_brk, -16, cur_errp);
  }
  lputs(", current = 0x", cur_errp);
  scm_intprint(scm_curbrk, -16, cur_errp);
  lputs("; ", cur_errp);
  scm_intprint(dif1, 10, cur_errp);
  if (dumped) {
    lputs(dif2 < 0 ? " - " : " + ", cur_errp);
    scm_intprint(dif2 < 0 ? -dif2 : dif2, 10, cur_errp);
  }
  lputs(".kiB\n", cur_errp);
}
