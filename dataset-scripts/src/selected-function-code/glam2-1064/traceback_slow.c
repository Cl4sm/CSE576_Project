void traceback_slow(glam2_aln *aln, int seq_pick, int strand, int end, data *d,
		    double temperature) {
  const fasta *f = &d->seqs.f[seq_pick];
  const int *seq = strand == '+' ? f->seq : f->rcseq;
  glam2_col *cols = aln->cols;
  double **ms = strand == '+' ? d->sm.match_scores : d->sm.rc_match_scores;
  double *ds = strand == '+' ? d->sm.delete_scores : d->sm.rc_delete_scores;
  double **dp_mat = strand == '+' ? d->sm.dp_mat : d->sm.rc_mat;
  int i = aln->width - 1;
  int j = end - 1;

  aln->strands[seq_pick] = strand;
  if (strand == 0)  /* the sequence is unaligned */
    return;

  while (i != -1 && j != -1) {
    double r = rand_dbl(dp_mat[i+1][j+1]);
    int a_count = aln->insert_counts[i];
    int b_count = aln->aligned_seq+1;  /* +1 */
    double insert_score = 1;

    if (dp_mat[i+1][j+1] < d->sm.underflow_flag)
      d->sm.underflow_flag = dp_mat[i+1][j+1];

    for (; j != -1; --j) {
      const double is = xpow(insert_score, 1 / temperature);
      const double match = ms[i][seq[j]] * dp_mat[i][j] * is;
      const double delete = ds[i] * dp_mat[i][j+1] * is;

      if (r < match) {  /* match */
	assert(seq[j] < d->alph.size);  /* ambiguous residue forbidden */
	cols[i].matches[seq_pick] = 1;
	cols[i].positions[seq_pick] = j;
	--i;
	--j;
	break;
      } else if (r < match + delete) {  /* deletion */
	cols[i].matches[seq_pick] = 0;
	cols[i].positions[seq_pick] = j+1;
	--i;
	break;
      }
      assert(i+1 < aln->width);
      r -= match + delete;
      insert_score *= beta_ratio_a(&d->scorer.i_prior, a_count, b_count);
      ++a_count;
    }
  }

  for (; i != -1; --i) {
    if (dp_mat[i+1][j+1] < d->sm.underflow_flag)
      d->sm.underflow_flag = dp_mat[i+1][j+1];
    cols[i].matches[seq_pick] = 0;
    cols[i].positions[seq_pick] = 0;
  }
}