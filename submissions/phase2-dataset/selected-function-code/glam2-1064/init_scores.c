void init_scores(data *d) {
  const int alph_size = d->m.alph_size;
  const int width = d->m.width;
  const double *bg = d->alph.prob;
  glam2_scorer *s = &d->scorer;
  int i, j;

  d->match_scores = xmalloc2(width, (alph_size+1) * sizeof(double));
  XMALLOC(d->delete_scores, width);
  XMALLOC(d->insert_scores, width);

  for (i = 0; i < width; ++i) {
    const int *residue_counts = d->m.residue_counts[i];
    const int match_count = sum_int(residue_counts, alph_size);
    const int delete_count = d->m.delete_counts[i];
    const int insert_count = d->m.insert_counts[i];
    const int aligned_seq = match_count + delete_count;

    const double ds = beta_ratio_a(&s->d_prior, delete_count, match_count);
    const double ms = beta_ratio_b(&s->d_prior, delete_count, match_count);
    const double is = beta_ratio_a(&s->i_prior, insert_count, aligned_seq);
    const double js = beta_ratio_b(&s->i_prior, insert_count, aligned_seq);

    dmix_ratios(&s->e_prior, d->match_scores[i], residue_counts);

    if (i+1 < width) {
      for (j = 0; j < alph_size; ++j)
        d->match_scores[i][j] = xlog(js * ms * d->match_scores[i][j] / bg[j]);
      d->delete_scores[i] = xlog(ds * js);
      d->insert_scores[i] = xlog(is);
    } else {  /* no insertions after last column: special case */
      for (j = 0; j < alph_size; ++j)
        d->match_scores[i][j] = xlog(ms * d->match_scores[i][j] / bg[j]);
      d->delete_scores[i] = xlog(ds);
      d->insert_scores[i] = -DBL_MAX;  /* overflow risk? */
    }

    /* overflow risk? */
    d->match_scores[i][alph_size] = -DBL_MAX;  /* mask character forbidden */
  }
}