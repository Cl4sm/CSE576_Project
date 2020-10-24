void dyn_prog(data *d, int strand) {
  const int width = d->m.width;
  const int seqlen = d->f.seqlen;
  const int *seq = strand == '+' ? d->f.seq : d->f.rcseq;
  const int *end = seq + seqlen;
  int i;

  for (i = 0; i < width; ++i) {
    /* tried to optimize this inner loop as much as possible */
    /* it seems surprisingly easy to beat gcc -O3 */
    const int *s = seq;
    const double *ms = d->match_scores[i];
    const double ds = d->delete_scores[i];
    const double is = d->insert_scores[i];
    const double *mat1 = d->dp_mat[i];
    double *mat2 = d->dp_mat[i+1];
    double m1 = *mat1;
    double m2 = *mat2;

    while (s != end) {
      ++mat1;
      ++mat2;
      const double match = ms[*s] + m1;
      m1 = *mat1;
      const double delete = ds + m1;
      m2 += is;
      if (match > m2)
	m2 = match;
      if (delete > m2)
	m2 = delete;
      *mat2 = m2;
      ++s;
    }
  }
}