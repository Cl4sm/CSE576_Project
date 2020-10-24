{
  FILE *fp;

  if ((fp = fopen(historyFilename, "w")) == 0) {
    fprintf(stderr, "Could not open history file '%s' for writing.  History will not be saved.\n", historyFilename);
    return;
  }

  histTOC2 = histTOC;
  for (histIndex = 0; histIndex < historyLength; histIndex++) {
    fprintf(fp, "%s\n", (char *) *histTOC2);
    histTOC2++;
  }

  fclose(fp);

  if (__DEBUG__)
    fprintf(stderr, "Finished writing new history file.\n");
}
