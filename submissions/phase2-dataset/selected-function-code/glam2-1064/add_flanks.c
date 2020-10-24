static void add_flanks(alignment *a, const mfasta *m, int want_extend) {
  const int left_tot = aln_tot_left(a, want_extend);
  const int right_tot = get_tot_right(a, m, want_extend);
  int left_cumu = 0;  /* only used for extended alignments */
  int right_cumu = 0;  /* only used for extended alignments */
  int i;
  size_t j = 0;

  for (i = 0; i < m->seqnum; ++i) {
    const fasta *f = &m->f[i];
    aligned_seq *s = &a->seqs[j];

    if (j != a->seq_num && strcmp(f->title, s->name) == 0) {
      const int left = xatoi(s->start) - 1;  /* zero-based coordinate */
      const int aln_len = strlen(s->seq);
      const int seq_len = strccnt(s->seq, '.');
      const int aln_end = left_tot + aln_len;
      const int seq_end = left + seq_len;
      const int right = f->seqlen - seq_end;
      const int left_extra = left_tot - left_cumu - left;
      const int right_extra = right_tot - right_cumu - right;
      const int left_beg = left_extra;
      const int right_beg = aln_end + right_cumu;
      const int left_end = left_beg + left;
      const int right_end = right_beg + right;
      const int tot_len = aln_end + right_tot;
      char *new_seq = xmalloc(tot_len+1);  /* space for the final 0 */

      memset(new_seq, '.', left_extra);
      int2char(new_seq + left_beg, f->seq, left);
      memset(new_seq + left_end, '.', left_cumu);
      memcpy(new_seq + left_tot, s->seq, aln_len);
      memset(new_seq + aln_end, '.', right_cumu);
      int2char(new_seq + right_beg, f->seq + seq_end, right);
      memset(new_seq + right_end, '.', right_extra);
      new_seq[tot_len] = 0;  /* terminate the string */

      free(s->seq);
      s->seq = new_seq;
      left_cumu += left * want_extend;
      right_cumu += right * want_extend;
      ++j;
    }
  }

  if (j != a->seq_num)
    die("%s: didn't find %s among the sequences\n",
	prog_name, a->seqs[j].name);
  assert(left_cumu == left_tot * want_extend);
  assert(right_cumu == right_tot * want_extend);
}