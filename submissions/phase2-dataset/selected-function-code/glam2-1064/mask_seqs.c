void mask_seqs(const alignment *a, int mask_char, FILE *in, FILE *out) {
  static const int name_width = 12;  /* max name width for glam2 */
  static const int line_size = 60;  /* line size for FASTA-format output */
  fasta f;
  size_t i = 0;

  while (fasta_read(&f, in) != EOF) {
    const aligned_seq *s = &a->seqs[i];
    /* mangle the title in the same way as glam2: */
    char *title = xstrdup(f.title);
    first_word(title);
    strtrunc(title, name_width);

    if (i != a->seq_num && strcmp(title, s->name) == 0) {
      int pos = xatoi(s->start)-1;
      size_t j;

      for (j = 0; s->seq[j] != 0; ++j) {
	if (pos >= f.seqlen)
	  break;  /* die with error message? */
	if (s->seq[j] != '.') {
	  if (a->key_positions[j] == '*')
	    f.seq[pos] = mask_char;
	  ++pos;
	}
      }

      ++i;
    }

    free(title);
    put_fasta(&f, line_size, out);
    free_fasta(&f);
  }

  if (i != a->seq_num)
    die("%s: didn't find %s among the sequences\n",
	prog_name, a->seqs[i].name);
}