void forward_slow(const glam2_aln *aln, int seq_pick, int strand, data *d,
		  double temperature) {
  const fasta *f = &d->seqs.f[seq_pick];
  const int *seq = strand == '+' ? f->seq : f->rcseq;
  double **ms = strand == '+' ? d->sm.match_scores : d->sm.rc_match_scores;
  double *ds = strand == '+' ? d->sm.delete_scores : d->sm.rc_delete_scores;
  double **dp_mat = strand == '+' ? d->sm.dp_mat : d->sm.rc_mat;
  double *convolver = d->sm.convolver;
  double *convolved = d->sm.convolved;
  double *rescale = strand == '+' ? &d->sm.dp_rescale : &d->sm.rc_rescale;
  int i, j;
  *rescale = 0;

  for (i = 0; i < aln->width; ++i) {
    const double r = *max_dbl(dp_mat[i], f->seqlen+1);
    if (r <= 0)
      die("%s: underflow in alignment i=%d\n", prog_name, i);
    div_dbl(ms[i], d->alph.size, r);
    ds[i] /= r;
    *rescale += xlog(r);

    dp_mat[i+1][0] = ds[i] * dp_mat[i][0];

    for (j = 0; j < f->seqlen; ++j) {
      dp_mat[i+1][j+1] =
        ms[i][seq[j]] * dp_mat[i][j]
	+ ds[i] * dp_mat[i][j+1];

      if (dp_mat[i+1][j+1] >= DBL_MAX)
        die("%s: overflow %g in alignment i=%d j=%d M=%.3g*%.3g D=%.3g*%.3g\n",
            prog_name, dp_mat[i+1][j+1], i, j, ms[i][seq[j]],
            dp_mat[i][j], ds[i], dp_mat[i][j+1]);
    }

    if (i+1 < aln->width) {  /* no insertions after last column */
      beta_convolver_a(&d->scorer.i_prior, convolver, f->seqlen+1,
		       aln->insert_counts[i], aln->aligned_seq+1);  /* +1 */
      pow_dbl(convolver, f->seqlen+1, 1 / temperature);
      if (d->a.algorithm == 2) {
#ifdef FFT
	fft_convolve(dp_mat[i+1], dp_mat[i+1], convolver, f->seqlen+1, &d->sm.fft);
#endif
	/* set negative numbers produced by FFT to zero: big fat kludge */
	truncate_min_dbl(dp_mat[i+1], f->seqlen+1, 0);
      } else {
	convolve(convolved, dp_mat[i+1], convolver, f->seqlen+1);
	COPY(dp_mat[i+1], convolved, f->seqlen+1);
      }
    }
  }
}