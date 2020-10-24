void scan_seq(data *d, int strand) {
  const int width = d->m.width;
  const int seqlen = d->f.seqlen;

  reinit_dp(d, d->f.seqlen);
  dyn_prog(d, strand);

  while (1) {
    /*
    print_mat(d->dp_mat, width+1, seqlen+1);
    */
    const double *last_row = d->dp_mat[width];
    const int end = max_dbl(last_row, seqlen+1) - last_row;
    const int start = traceback(d, end, strand);
    /* require at least 1 match, to prevent infinite loop: */
    if (sum_int(d->hit_matches, width) == 0)
      break;
    if (d->hit_num == d->a.hit_num) {
      if (d->hit_num == 0 || last_row[end] <= d->hits[0].score)
	break;
      POP_HEAP(d->hits, d->hit_num, cmp_alignment);
      --d->hit_num;
      free_alignment(d->hits + d->hit_num);
    }
    init_alignment(d->hits + d->hit_num, d, strand);
    ++d->hit_num;
    PUSH_HEAP(d->hits, d->hit_num, cmp_alignment);
    recalculate(d, start, strand);
  }
}