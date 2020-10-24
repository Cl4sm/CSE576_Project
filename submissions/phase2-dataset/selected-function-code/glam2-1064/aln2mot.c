void aln2mot(motif *m, const alignment *a, int alph_size, const int *encode) {
  const int columns = strlen(a->key_positions);
  const int width = strcnt(a->key_positions, '*');
  int **residue_counts = xcalloc2(width, alph_size, sizeof(int));  /* zero */
  int *delete_counts = xcalloc(width, sizeof(int));  /* zero fill */
  int *insert_counts = xcalloc(width, sizeof(int));  /* zero fill */
  int i, j, k;

  for (i = 0; i != a->seq_num; ++i) {
    const char *seq = a->seqs[i].seq;
    k = 0;

    for (j = 0; j < columns; ++j) {
      const int c = (unsigned char)seq[j];  /* is this OK? */
      if (a->key_positions[j] == '*') {
	if (c != '.') {
	  if (encode[c] == alph_size)
	    die("%s: error reading motif file: ambiguous residue %c in aligned column\n", prog_name, c);
	  ++residue_counts[k][encode[c]];
	}
	delete_counts[k] += c == '.';
	++k;
      } else {
	assert(k > 0);
	assert(k < width);
	insert_counts[k-1] += c != '.';
      }
    }
  }  

  m->width = width;
  m->alph_size = alph_size;
  m->seq_num = a->seq_num;
  m->residue_counts = residue_counts;
  m->delete_counts = delete_counts;
  m->insert_counts = insert_counts;
}