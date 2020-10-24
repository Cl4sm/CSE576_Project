void init_sm(score_matrices *sm, const data *d) {
  const int alph_size = d->alph.size;
  const int max_width = d->a.max_width;
  const int max_seqlen = d->seqs.maxlen;
  int i;

  sm->match_scores = xmalloc2(max_width, (alph_size+1) * sizeof(double));
  XMALLOC(sm->delete_scores, max_width);
  XMALLOC(sm->insert_scores, max_width);

  for (i = 0; i < max_width; ++i)  /* zero score for mask character */
    sm->match_scores[i][alph_size] = 0;

  /* dp_mat needs one extra row and column: */
  sm->dp_mat = xmalloc2(max_width+1, (max_seqlen+1) * sizeof(double));
  /* boundary condition for short-in-long alignment: */
  set_dbl(sm->dp_mat[0], max_seqlen+1, 1);

  if (d->a.two_strands) {
    sm->rc_match_scores = xmalloc2(max_width, (alph_size+1) * sizeof(double));
    XMALLOC(sm->rc_delete_scores, max_width);

    for (i = 0; i < max_width; ++i)  /* zero score for mask character */
      sm->rc_match_scores[i][alph_size] = 0;

    sm->rc_mat = xmalloc2(max_width+1, (max_seqlen+1) * sizeof(double));
    /* boundary condition for short-in-long alignment: */
    set_dbl(sm->rc_mat[0], max_seqlen+1, 1);
  }
}