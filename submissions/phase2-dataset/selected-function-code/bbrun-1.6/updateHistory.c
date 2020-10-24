void updateHistory(char *newHistoryItem) 
{
  int duplicate = -1;
  int historyIndex;
  char *item;
  u32 *histTransit;		// Before, we would copy the data around, now we play around
 				// with the pointers, which should be more efficient. 

  if (__DEBUG__)
    fprintf(stderr, "Adding '%s' to history... ", newHistoryItem);

  if (historyLength == -1)
    return;

  // See if the command is in history already
  histTOC2 = histTOC;
  for (historyIndex = 0; historyIndex < historyLength; historyIndex++) {
    if (strncmp((char *) *histTOC2, newHistoryItem, MAXCMDLEN) == 0) {
      duplicate = historyIndex;
      break;
    }
    histTOC2++;
  }

  if (duplicate != -1) {
    if (__DEBUG__)
      fprintf(stderr, " duplicate of item [%02d].\n", duplicate);

    if (duplicate != (historyLength - 1)) {	// If the duplicate entry is not at the end 
      histTransit = (u32 *) (histTOC + duplicate);

      // Shift each entry forward
      for (historyIndex = duplicate; historyIndex < historyLength - 1; historyIndex++) {
	*histTOC2 = *(histTOC2+1); 
	histTOC2++;
      }

      // put duplicate at the end
      histTOC2 = histTOC + historyLength - 1;
      *histTOC2 = (u32) histTransit; 
    }
  } else {
    // The command is NOT in the history already, so add it
    if (__DEBUG__)
      fprintf(stderr, " new history item.\n");

    historyLength++;

    // Set the last item of the history to be the new command
    histTOC = realloc(histTOC, sizeof(u32) * historyLength);
    histTOC2 = histTOC + historyLength - 1;

    item = malloc(MAXCMDLEN + 1);

    strncpy(item, newHistoryItem, MAXCMDLEN + 1);
    item[strlen(item)] = 0x0;
    *histTOC2 = (u32) item;
  }

  if (__DEBUG__) {
    histTOC2 = histTOC;
    fprintf(stderr, "History shuffling complete.  New history is: \n");
    for (histIndex = 0; histIndex < historyLength; histIndex++) {
      fprintf(stderr, "\t[%02d] '%s'\n", histIndex, (char *) *histTOC2);
      histTOC2++;
    }
  }

  writeHistory();
}
