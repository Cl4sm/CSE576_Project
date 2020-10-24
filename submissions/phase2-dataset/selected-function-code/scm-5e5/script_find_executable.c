     const char *name;
{
  char tbuf[MAXPATHLEN];
  int i = 0;
  FILE *f;

  /* fprintf(stderr, "s_f_e checking access %s ->%d\n", name, access(name, X_OK)); fflush(stderr); */
  if (access(name, X_OK)) return 0L;
  f = fopen(name, "r");
  if (!f) return 0L;
  if ((fgetc(f)=='#') && (fgetc(f)=='!')) {
    while (1) switch (tbuf[i++] = fgetc(f)) {
    case ' ':
      if (1==i) {i--; break;}
    case '\t':case '\r':case '\f':
    case EOF:
      tbuf[--i] = 0;
      fclose(f);
      if (0==i) return 0L;
      return scm_cat_path(0L, tbuf, 0L);
    }
  }
  fclose(f);
  return scm_cat_path(0L, name, 0L);
}
