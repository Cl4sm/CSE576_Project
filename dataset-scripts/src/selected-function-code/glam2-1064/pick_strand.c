int pick_strand(const glam2_aln *aln, const int seq_pick, const data *d) {
  const int width = aln->width;
  const int seq_len = d->seqs.f[seq_pick].seqlen;
  double **dp_mat = d->sm.dp_mat;
  double **rc_mat = d->sm.rc_mat;
  const double dp_rescale = d->sm.dp_rescale;
  const double rc_rescale = d->sm.rc_rescale;
  double tot[3];
  double log_scale[3];
  double max;
  int i, r;

  tot[0] = sum_dbl(dp_mat[width], seq_len+1);
  assert(tot[0] > 0);
  log_scale[0] = dp_rescale;

  if (d->a.two_strands) {
    tot[1] = sum_dbl(rc_mat[width], seq_len+1);
    assert(tot[1] > 0);
    log_scale[1] = rc_rescale;
  } else {
    tot[1] = 0;
    log_scale[1] = dp_rescale;
  }

  if (aln->aligned_seq >= d->a.min_seqs) {
    tot[2] = 1;
    log_scale[2] = 0;
  } else {
    tot[2] = 0;
    log_scale[2] = dp_rescale;
  }

  /*
  puts("tot:");
  print_vec(tot, 3);
  puts("log_scale:");
  print_vec(log_scale, 3);
  */

  max = *max_dbl(log_scale, 3);
  for (i = 0; i < 3; ++i)
    tot[i] *= xexp(log_scale[i] - max);
  r = pick_dbl(tot, 3) - tot;

  if (r == 0)
    return '+';
  else if (r == 1)
    return '-';
  else
    return 0;
}