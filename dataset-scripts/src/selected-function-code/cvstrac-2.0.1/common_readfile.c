char *common_readfile(const char *zFilename) {
  FILE *fp;
  char *zContent = NULL;
  size_t n;

  if ((fp = fopen(zFilename, "r")) != NULL) {
    fseek(fp, 0, SEEK_END);
    if ((n = ftell(fp)) > 0) {
      if ((zContent = (char *)malloc(n+1)) == NULL) {
        fclose(fp);
        return NULL;
      }
      fseek(fp, 0, SEEK_SET);
      if ((n = fread(zContent, 1, n, fp)) == 0) {
        free(zContent);
        fclose(fp);
        return NULL;
      }
      zContent[n] = '\0';
    }
    else {
      zContent = strdup("");
    }
    fclose(fp);
  }
  return zContent;
}