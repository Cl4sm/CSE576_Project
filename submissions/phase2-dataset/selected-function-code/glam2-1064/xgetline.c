size_t xgetline(char **lineptr, size_t *n, FILE *stream) {
  size_t i = 0;  /* number of characters read so far */
  int c;  /* this character */
  assert(lineptr != NULL);

  if (*lineptr == NULL || *n == 0) {
    *n = 1;  /* should probably use a bigger starting size */
    *lineptr = XREALLOC(*lineptr, *n);
  }

  while((c = getc(stream)) != EOF) {
    ++i;
    if (i == *n) {
      *n = *n * 2;
      if (*n <= i)  /* overflow */
        die("%s: line too long: >= %lu\n", prog_name, (unsigned long)i);
      *lineptr = XREALLOC(*lineptr, *n);
    }
    (*lineptr)[i-1] = c;  /* int -> char! */
    if (c == '\n')
      break;
  }

  if (ferror(stream))
    die("%s: error reading file: %s\n", prog_name, strerror(errno));
  assert(i < *n);
  (*lineptr)[i] = '\0';
  return i;
}