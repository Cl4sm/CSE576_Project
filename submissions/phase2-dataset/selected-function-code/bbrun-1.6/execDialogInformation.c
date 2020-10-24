{
  gchar *workingPath = '\0';
  gchar *command = '\0';
  char backgroundCommand[MAXCMDLEN + 3];
  char *originalWorkingDirectory = '\0';

  command = (gchar *) gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo)->entry));
  if (command[0] != '\0') {
    bzero(backgroundCommand, MAXPATHLEN + 3);
    if (__DEBUG__)
      fprintf(stderr, "Command retrieved from input box as '%s'.\n", command);
    command = strtok(command, "\n");
    updateHistory(command);
    strncpy(backgroundCommand, command, MAXCMDLEN);
    strcat(backgroundCommand, " &");
    if (advancedMode) {
      originalWorkingDirectory = getcwd(NULL, 0);
      workingPath = (gchar *) gtk_entry_get_text(GTK_ENTRY(workingDirectoryEntry));
      if (__DEBUG__) {
        fprintf(stderr, "Original working directory is '%s'\n", originalWorkingDirectory);
        fprintf(stderr, "Execution working path is '%s'\n", workingPath);
        fprintf(stderr, "Command to execute is '%s'\n", backgroundCommand);
      }
      if (workingPath[0] == '\0')
        system(backgroundCommand);
      else {
        if (chdir(workingPath) == 0) {
          system(backgroundCommand);
          if (chdir(originalWorkingDirectory) != 0)
            perror(originalWorkingDirectory);
        } else
          perror(workingPath);
      }
      if (originalWorkingDirectory) 
	free(originalWorkingDirectory);
    } else 
      system(backgroundCommand);
  }
}
