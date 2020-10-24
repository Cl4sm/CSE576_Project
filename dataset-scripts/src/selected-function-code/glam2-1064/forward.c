void forward(const glam2_aln *aln, int seq_pick, int strand, data *d) {
  const fasta *f = &d->seqs.f[seq_pick];
  const int *seq = strand == '+' ? f->seq : f->rcseq;
  const int *end = seq + f->seqlen;
  double **ms = strand == '+' ? d->sm.match_scores : d->sm.rc_match_scores;
  double *ds = strand == '+' ? d->sm.delete_scores : d->sm.rc_delete_scores;
  double *is = d->sm.insert_scores;
  double **dp_mat = strand == '+' ? d->sm.dp_mat : d->sm.rc_mat;
  double *rescale = strand == '+' ? &d->sm.dp_rescale : &d->sm.rc_rescale;
  double r = 1;  /* rescaling factor for one row */
  int i;
  assert(is[aln->width-1] == 0);
  *rescale = 0;

  for (i = 0; i < aln->width; ++i) {
    if (r <= 0)
      die("%s: underflow in alignment i=%d\n", prog_name, i);
    div_dbl(ms[i], d->alph.size, r);
    ds[i] /= r;
    *rescale += xlog(r);

    /* tried to optimize this inner loop as much as possible */
    const int *s = seq;
    const double *msi = ms[i];
    const double dsi = ds[i];
    const double isi = is[i];
    const double *mat1 = dp_mat[i];
    double *mat2 = dp_mat[i+1];
    double m1 = *mat1;
    double m2 = dsi * m1;
    *mat2 = m2;
    r = m2;

    while (s != end) {
      ++mat1;
      ++mat2;
      m2 *= isi;
      m2 += msi[*s] * m1;
      m1 = *mat1;
      m2 += dsi * m1;
      *mat2 = m2;

      if (m2 > r)
	r = m2;
      if (m2 >= DBL_MAX)  /* this check doesn't seem to slow it down */
	die("%s: overflow %g in alignment i=%d isi=%.3g dsi=%.3g msi=%.3g m1=%.3g\n",
	    prog_name, m2, i, isi, dsi, msi[*s], m1);

      ++s;
    }
  }
}