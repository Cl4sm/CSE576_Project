static void get_score_matrices(score_matrices *sm, const glam2_aln *aln,
			       glam2_scorer *s) {
  double **match_scores = sm->match_scores;
  double *delete_scores = sm->delete_scores;
  double *insert_scores = sm->insert_scores;
  int i, j;

  for (i = 0; i < aln->width; ++i) {  /* loop over aligned columns */
    const int alph_size = aln->cols[i].alph_size;  /* should be in sm */
    const int *residue_counts = aln->cols[i].emission_counts;
    const int match_count = aln->cols[i].match_count;
    const int delete_count = aln->cols[i].delete_count;
    const int insert_count = aln->insert_counts[i];
    const int aligned_seq = aln->aligned_seq;

    const double ds = beta_ratio_a(&s->d_prior, delete_count, match_count);
    const double ms = beta_ratio_b(&s->d_prior, delete_count, match_count);
    const double is = beta_ratio_a(&s->i_prior, insert_count, aligned_seq);
    const double js = beta_ratio_b(&s->i_prior, insert_count, aligned_seq);

    assert(residue_counts[alph_size] == 0);  /* no mask characters */

    dmix_ratios(&s->e_prior, match_scores[i], residue_counts);

    if (i+1 < aln->width) {
      for (j = 0; j < alph_size; ++j)
	match_scores[i][j] *= js * ms / s->bg.probs[j];
      delete_scores[i] = ds * js;
      insert_scores[i] = is;
    } else {  /* no insertions after last column: special case */
      for (j = 0; j < alph_size; ++j)
	match_scores[i][j] *= ms / s->bg.probs[j];
      delete_scores[i] = ds;
      insert_scores[i] = 0;
    }
  }
}