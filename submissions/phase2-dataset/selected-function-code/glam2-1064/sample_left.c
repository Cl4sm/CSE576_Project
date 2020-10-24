void sample_left(glam2_aln *aln, data *d, const double temperature) {
  column_sampler *s = &d->col_sampler;
  const int del_flag = rand_dbl(1) < s->del_probs[aln->width - 1];
  const int col_pick = rand_int(aln->width - 1);
  int match_cols, new_col;
  if (d->a.profile)
    fprintf(d->out, "column sample: side=left, delete=%d, col=%d\n",
	    del_flag, col_pick);
  get_offsets(s->offsets, aln, col_pick+1);
  col_copy(&s->col, &aln->cols[col_pick]);
  if (del_flag)
    rotate_out(aln, col_pick);  /* Remove column & decrement width */
  else if (aln->width == d->a.max_width)
    return;
  s->width = aln->width;
  left_fits(s, aln);
  col_scan_left(s, aln, d);
  match_cols = col_count_left(s, aln);
  if (aln->width < d->a.min_width)
    match_cols = 0;  /* force column re-insertion */
  new_col = pick_new_col(s, match_cols, temperature);
  if (new_col >= s->width)
    return;
  col_put_left(s, aln, new_col);
  count_residues(&s->col, aln, &d->seqs);
  col_copy(&aln->cols[aln->width], &s->col);
  rotate_in(aln, new_col);
}