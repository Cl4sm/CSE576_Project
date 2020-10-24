int traceback(data *d, int end, int strand) {
  const int width = d->m.width;
  const int *seq = strand == '+' ? d->f.seq : d->f.rcseq;
  int i = width - 1;
  int j = end - 1;

  while (i != -1 && j != -1) {
    const double match = d->forbidden[i][j] == 0 ?
      d->match_scores[i][seq[j]] + d->dp_mat[i][j] : -DBL_MAX;
    const double delete = d->delete_scores[i] + d->dp_mat[i][j+1];
    const double insert = d->insert_scores[i] + d->dp_mat[i+1][j];

    if (match >= delete && match >= insert) {  /* match */
      assert(seq[j] < d->alph.size);  /* ambiguous residue forbidden */
      d->hit_matches[i] = 1;
      d->hit_positions[i] = j;
      d->forbidden[i][j] = 1;
      --i;
      --j;
    } else if (delete >= insert) {  /* deletion */
      d->hit_matches[i] = 0;
      d->hit_positions[i] = j+1;
      --i;
    } else {  /* insertion */
      assert(i+1 != width);
      --j;
    }
  }

  for (; i != -1; --i) {
    d->hit_matches[i] = 0;
    d->hit_positions[i] = 0;
  }

  return j+1;
}