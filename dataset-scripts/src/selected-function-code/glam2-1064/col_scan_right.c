void col_scan_right(column_sampler *s, const glam2_aln *aln, data *d) {
  const int aligned_seq = aln->aligned_seq;
  const int off_tot = sum_offsets(s->offsets, aln);
  const int res_tot = off_tot + s->col.match_count;
  double *score_ptr = s->scores;
  int i, j;

  for (i = 0; i < aln->width; ++i) {
    const double old_score = i != aln->width-1 ?
      insertion_score(&d->scorer, aln->insert_counts[i], aligned_seq) : 0;
    int left_inserts = off_tot;
    int right_inserts = i != aln->width-1 ?
      aln->insert_counts[i] - res_tot : 0;
    col_put_right(s, aln, i);

    for (j = 0; j < s->fits[i]; ++j) {
      assert(score_ptr-s->scores < d->seqs.maxlen+d->a.max_width);
      count_residues(&s->col, aln, &d->seqs);
      *score_ptr = column_score(&d->scorer, &s->col)
	+ insertion_score(&d->scorer, left_inserts, aligned_seq);
      if (i != aln->width-1)
	*score_ptr += insertion_score(&d->scorer, right_inserts, aligned_seq)
	  - old_score;
      if (s->col.emission_counts[s->col.alph_size])
	*score_ptr = -DBL_MAX;  /* forbid ambiguous residues */
      add_int(s->col.positions, aln->seq_num, 1);  /* add 1 to positions */
      left_inserts += aligned_seq;
      right_inserts -= aligned_seq;
      ++score_ptr;
    }
  }
}