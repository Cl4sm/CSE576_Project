SCM lroom(opt)
     SCM opt;
{
  scm_intprint(cells_allocated, -10, cur_errp);
  lputs(" out of ", cur_errp);
  scm_intprint(heap_cells, -10, cur_errp);
  lputs(" cells in use, ", cur_errp);
  scm_intprint(mallocated, -10, cur_errp);
  lputs(".B allocated (of ", cur_errp);
  scm_intprint(mtrigger, 10, cur_errp);
  lputs(")\n", cur_errp);
  if (!UNBNDP(opt)) {
#ifndef LACK_SBRK
    if (scm_init_brk) scm_brk_report();
#endif
    scm_ecache_report();
    heap_report(); lputc('\n', cur_errp);
    gra_report();
    stack_report();
  }
  return UNSPECIFIED;
}
