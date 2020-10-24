  char buf[MAXCMDLEN];
  char *item;
  FILE *fp;

  histTOC = malloc(sizeof(u32) * 1);
  histTOC2 = histTOC;

  if ((fp = fopen(historyFilename, "r")) == 0) {
    if ((fp = fopen(historyFilename, "w")) == 0) {
      fprintf(stderr, "Error creating history file '%s'.\n", historyFilename);
      historyLength = -1;
      return;
    }

    if (__DEBUG__) 
      fprintf(stderr, "Just finished creating a new history file '%s'.\n", historyFilename);

    fclose(fp);

    return;
  }

  while (fgets(buf, MAXCMDLEN, fp) != 0) {
    if (buf != NULL) {
      historyLength++;

      histTOC = realloc(histTOC, sizeof(u32) * historyLength);
      histTOC2 = histTOC + historyLength - 1;

      item = malloc(strlen(buf));
      strncpy(item, buf, strlen(buf));
      item[strlen(buf)- 1] = 0x0;			// Remove the newline char

      *histTOC2 = (u32) item;
    } else {
      // scott@furt.com, This is a NULL line, which should NEVER happen.  Stop any further processing, 
      // because chances are very good that the rest of the file is corrupt too.
      if (__DEBUG__)
	fprintf(stderr, "Null line encountered in history file.  Stopping at %d history items.\n", historyLength);
      break;
    }
  }

  fclose(fp);

  if (__DEBUG__) {
    histTOC2 = histTOC;

    fprintf(stderr, "Finished reading old history file. History items are: \n");
    for (histIndex = 0; histIndex < historyLength; histIndex++) {
      fprintf(stderr, "\t[%02d] '%s'\n", histIndex, (char *) *histTOC2);
      histTOC2++;
    }
  }

//  for (histTOC2 = histTOC; *histTOC2; histTOC2++)
//    free(*histTOC2);
//  free(histTOC);

}
