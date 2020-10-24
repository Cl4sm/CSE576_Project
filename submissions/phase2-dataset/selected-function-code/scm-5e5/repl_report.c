{
  if (verbose > 2) {
    lfflush(cur_outp);
    lputs(";Evaluation took ", cur_errp);
    scm_intprint(time_in_msec(INUM(my_time())-rt), -10, cur_errp);
    lputs(".ms (", cur_errp);
    scm_intprint(time_in_msec(gc_time_taken), -10, cur_errp);
    lputs(".ms in gc) ", cur_errp);
    scm_intprint(cells_allocated - lcells_allocated, -10, cur_errp);
    lputs(" cells work, ", cur_errp);
    scm_env_work += scm_ecache_len - scm_ecache_index;
    scm_intprint(scm_env_work, -10, cur_errp);
    lputs(" env, ", cur_errp);
    scm_intprint(mallocated - lmallocated, -10, cur_errp);
    lputs(".B other\n", cur_errp);
    if (verbose > 3) {
      lputc(';', cur_errp);
      scm_intprint(scm_gcs, -10, cur_errp);
      lputs( " gc, ", cur_errp);
      scm_intprint(scm_egcs, -10, cur_errp);
      lputs( " ecache gc, ", cur_errp);
      scm_intprint(scm_clo_moved, -10, cur_errp);
      lputs(" env migrated from closures, ", cur_errp);
      scm_intprint(scm_stk_moved, -10, cur_errp);
      lputs(" from stack\n", cur_errp);
    }
    lfflush(cur_errp);
  }
}
