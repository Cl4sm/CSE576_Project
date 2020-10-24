static void aln_do_extend(alignment *a, const alignment *b) {
  const char *kp = b->key_positions;
  const size_t seqnum = b->seq_num;
  size_t apos = 0;
  size_t s, t, bpos, bend, boff;

  for (bpos = 0; ; bpos = bend) {
    for (s = 0; s != seqnum; ++s)  /* fill in the key position */
      a->seqs[s].seq[apos] = b->seqs[s].seq[bpos];
    ++apos;
    ++bpos;

    if (kp[bpos] == 0)
      break;

    assert(strchr(kp + bpos, '*') != NULL);
    bend = strchr(kp + bpos, '*') - kp;  /* next key position */

    for (s = 0; s != seqnum; ++s) {  /* fill in the insertions */
      const char *bseq = b->seqs[s].seq;

      for (boff = bpos; boff != bend; ++boff)
	if (bseq[boff] != '.') {
	  for (t = 0; t != seqnum; ++t)  /* put gaps in all other sequences */
	    a->seqs[t].seq[apos] = t == s ? bseq[boff] : '.';
	  ++apos;
	}
    }
  }

  for (s = 0; s != seqnum; ++s)  /* terminate the strings */
    a->seqs[s].seq[apos] = 0;
}