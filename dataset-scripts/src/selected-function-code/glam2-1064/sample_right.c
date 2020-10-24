void sample_right(glam2_aln *aln, data *d, const double temperature) {
  column_sampler *s = &d->col_sampler;
  const int del_flag = rand_dbl(1) < s->del_probs[aln->width - 1];
  const int col_pick = rand_int(aln->width - 1) + 1;
  int match_cols, new_col;
  if (d->a.profile)
    fprintf(d->out, "column sample: side=right, delete=%d, col=%d\n",
	    del_flag, col_pick);
  get_offsets(s->offsets, aln, col_pick);
  col_copy(&s->col, &aln->cols[col_pick]);
  if (del_flag)
    rotate_out(aln, col_pick);  /* Remove column & decrement width */
  else if (aln->width == d->a.max_width)
    return;
  s->width = aln->width;
  right_fits(s, aln, &d->seqs);
  col_scan_right(s, aln, d);
  match_cols = col_count_right(s, aln);
  if (aln->width < d->a.min_width)
    match_cols = 0;  /* force column re-insertion */
  new_col = pick_new_col(s, match_cols, temperature);
  if (new_col >= s->width)
    return;
  col_put_right(s, aln, new_col);
  count_residues(&s->col, aln, &d->seqs);
  col_copy(&aln->cols[aln->width], &s->col);
  rotate_in(aln, new_col+1);
}