Imlib_Font *load_font(char *prefname, char **flist_) {
  Imlib_Font *f = NULL;
  char **flist = flist_;
  if (prefname) {
    f = imlib_load_font_nocase(prefname);
    if (!f) {
      int i,np; char **s;
      fprintf(stderr, "warning: could not find font '%s' in the font path:\n",prefname);
      s = imlib_list_font_path(&np);
      for (i=0; i < np; ++i) fprintf(stderr, "  %s\n", s[i]);
    } else {
      printf("loaded font %s\n", prefname);
    }
  }
  if (!f) {
    for (; *flist; ++flist) {
      if ((f = imlib_load_font_nocase(*flist))) {
        printf("loaded font %s\n", *flist); break;
      }
    }
    if (!f) {
      fprintf(stderr, "could not load a default ttf font .. I tried ");
      flist = flist_;
      for (; *flist; ++flist)
        fprintf(stderr, "'%s'%s", *flist, (flist[1]?", ":""));
      fprintf(stderr, "\nUse the --font* options to change the fontpath/fontnames\n");
    }
  }
  return f;
}