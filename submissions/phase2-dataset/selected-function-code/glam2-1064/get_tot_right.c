static int get_tot_right(const alignment *a, const mfasta *m, int want_sum) {
  int tot = 0;
  int i;
  size_t j = 0;

  for (i = 0; i < m->seqnum; ++i) {
    const fasta *f = &m->f[i];
    const aligned_seq *s = &a->seqs[j];

    if (j != a->seq_num && strcmp(f->title, s->name) == 0) {
      const int left = xatoi(s->start) - 1;  /* zero-based coordinate */
      const int seq_len = strccnt(s->seq, '.');
      const int seq_end = left + seq_len;
      const int right = f->seqlen - seq_end;
      if (want_sum)
	tot += right;
      else  /* want maximum */
	if (right > tot)
	  tot = right;
      ++j;
    }
  }

  return tot;
}