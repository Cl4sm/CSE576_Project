void init_background(prob_vec *bg, const data *d) {
  const int alph_size = d->alph.size;
  const double bg_pseudo = d->a.bg_pseudo * alph_size;
  int *counts = xcalloc(alph_size+1, sizeof(int));  /* zero fill */
  double *probs = xmalloc(alph_size * sizeof(double));
  double *log_probs = xmalloc(alph_size * sizeof(double));
  int tot;
  int i;

  count_mfasta(&d->seqs, counts);

  if (d->a.two_strands)
    for (i = 0; i < (alph_size+1)/2; ++i)
      counts[i] = counts[alph_size-i-1] = counts[i] + counts[alph_size-i-1];

  tot = sum_int(counts, alph_size);

  for (i = 0; i < alph_size; ++i)
    probs[i] = (counts[i] + bg_pseudo * d->alph.prob[i]) / (tot + bg_pseudo);

  for (i = 0; i < alph_size; ++i)
    log_probs[i] = xlog(probs[i]);

  bg->dim = alph_size;
  bg->counts = counts;
  bg->probs = probs;
  bg->log_probs = log_probs;
}