void alphabet_read(alphabet *a, FILE *fp) {
  int c;
  int state = 0;
  double tot;

  a->size = 0;
  set_int(a->encode, UCHAR_MAX+1, -1);
  a->prob = NULL;

  while((c = getc(fp)) != EOF) {
    if (c == '#') {  /* start of comment */
      fscanf(fp, "%*[^\n]");  /* skip line */
    } else if (c == '\n') {
      state = 0;
    } else if (isspace(c)) {
      if (state == 1) {
	double p = 0;
        const int r = fscanf(fp, "%lf", &p);
        if (r == 1 && p <= 0)
          die("%s: error reading alphabet file: abundance %g should be > 0\n",
              prog_name, p);
        a->prob[a->size - 1] = p;
        fscanf(fp, "%*[^\n]");  /* skip line */
      }
    } else {
      if (state == 0) {
	++a->size;
	a->decode[a->size - 1] = c;
        a->prob = XREALLOC(a->prob, a->size);
        a->prob[a->size - 1] = 1;  /* default abundance */
        state = 1;
      }
      if (a->encode[c] != -1 && a->encode[c] != a->size - 1)
	die("%s: error reading alphabet file: found %c twice\n",
	    prog_name, c);
      a->encode[c] = a->size - 1;
    }
  }
  if (ferror(fp))
    die("%s: error reading alphabet file: %s\n", prog_name, strerror(errno));

  if (a->size <= 1)  /* disallow size zero alphabet */
    die("%s: error reading alphabet file: no alphabet\n", prog_name);

  --a->size;  /* last line has the wildcard symbol */

  for (c = 0; c <= UCHAR_MAX; ++c)
    if (a->encode[c] == -1)
      a->encode[c] = a->size;

  tot = sum_dbl(a->prob, a->size);
  div_dbl(a->prob, a->size, tot);
}