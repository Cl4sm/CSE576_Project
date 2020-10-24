void recalculate(data *d, int start, int strand) {
  const int width = d->m.width;
  const int seqlen = d->f.seqlen;
  const int *seq = strand == '+' ? d->f.seq : d->f.rcseq;
  int end = start;
  int i, j;

  for (i = 0; i < width; ++i)
    for (j = start; j < seqlen; ++j) {
      const double match = d->forbidden[i][j] == 0 ?
	d->match_scores[i][seq[j]] + d->dp_mat[i][j] : -DBL_MAX;
      const double delete = d->delete_scores[i] + d->dp_mat[i][j+1];
      const double insert = d->insert_scores[i] + d->dp_mat[i+1][j];
      const double new = max3(match, delete, insert);

      if (new != d->dp_mat[i+1][j+1]) {
	d->dp_mat[i+1][j+1] = new;
	if (end < j)
	  end = j;
      } else
	if (j > end)
	  break;
    }
}