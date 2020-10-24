  static int
exh_shell_command(char *command, int pager_f)
                    /* command to be executed by the shell specified by
                     * the "SHELL" environment variable.  the default shell
                     * is `EXH_DEFAULT_SHELL'. */
                    /* if `pager != 0' the output is piped into the
                     * pager specified by the "PAGER" environment.  the
                     * default pager is `HE_DEFAULT_PAGER'. */
  /* we won't use the "-c"-switch (available for most shells), instead we're
   * gonna pipe the command into the shell.
   */
{
  int shell_pid, pager_pid;
  int shell_status, pager_status;
  int shell_x, pager_x;
  int pipe1[2], pipe2[2];
  char *shell[64], *pager[64], *p;
  int i;

  tio_suspend();
  tio_clear();
  tio_flush();
  if (!(*shell = getenv("SHELL"))) *shell = EXH_DEFAULT_SHELL;
  *shell = strdup(*shell);
  if (!(*pager = getenv("PAGER"))) *pager = HE_DEFAULT_PAGER;
  *pager = strdup(*pager);
  /* break `*shell' and `*pager' down into whitespace separated
   * substrings.  it is *not* possible to mask whitespace characters in any
   * way. */
  for (i = 0; *shell[i]; shell[++i] = p) {
    for (p = shell[i]; *p && *p != ' ' && *p != '\t'; ++p);
    if (*p) for (*p++ = 0; *p == ' ' || *p == '\t'; ++p);
  }
  shell[i] = 0;
  for (i = 0; *pager[i]; pager[++i] = p) {
    for (p = pager[i]; *p && *p != ' ' && *p != '\t'; ++p);
    if (*p) for (*p++ = 0; *p == ' ' || *p == '\t'; ++p);
  }
  pager[i] = 0;
  pipe1[0] = pipe1[1] = -1;
  if (pipe(pipe1) ? 1 : pager_f ? pipe(pipe2) : 0) {
    he_message(1, "INTERNAL ERROR: `pipe()' failed.");
    if (pipe1[0] > 0) { close(pipe1[0]); close(pipe1[1]); }
    goto exit_exh_shell_command;
  }
  /* execute the shell (and the pager)
   */
  if ((shell_pid = fork()) > 0) { /* parent */
    close(pipe1[0]);
    if (pager_f) { /* start up the pager */
      if ((pager_pid = fork()) > 0) { /* parent */
        close(pipe2[0]);
        close(pipe2[1]);
        /* send the command to the shell */
        if (write(pipe1[1], command, strlen(command)) != strlen(command) ||
	    write(pipe1[1], "\n", 1) != 1) {
	  close(pipe1[1]);
	  he_message(1, "couldn't send the `%s' command to the pager", command);
	  close(pipe2[0]);
	  close(pipe2[1]);
	  pager_f = 0;
	} else {
        close(pipe1[1]);
        do if (waitpid(shell_pid, &shell_status, 0) >= 0) break;
          while (errno == ERESTARTSYS);
        do if (waitpid(pager_pid, &pager_status, 0) >= 0) break;
          while (errno == ERESTARTSYS);
        if ((pager_x = WEXITSTATUS(pager_status))) {
          if (pager_x == EXIT_EXEC_FAILED) {
            he_message(1, "couldn't start pager program `%s'", *pager);
            close(pipe2[0]);
            close(pipe2[1]);
            pager_f = 0;  /* we clear the `pager_f'-flag, so the
                           * "press any key"-requester gets displayed. */
          } else
            he_message(1, "%s exited with code %i", *pager, pager_x);
        }
        if ((shell_x = WEXITSTATUS(shell_status))) {
          if (shell_x == EXIT_EXEC_FAILED)
            he_message(1, "couldn't start shell `%s'", *shell);
          else
            he_message(1, "%s exited with code %i", *shell, shell_x);
        }
        if (!shell_x && !pager_x)
          he_message(0, "shell command done");
	}
      } else if (pager_pid < 0) { /* `fork()' failed */
        close(pipe2[0]);
        close(pipe2[1]);
        he_message(1, "INTERNAL ERROR: `fork()' failed.");
        /* hmm... i think it's better not to send the command in case the
         * pager failed. */
        close(pipe1[1]);
        do if (waitpid(shell_pid, &shell_status, 0) >= 0) break;
          while (errno == ERESTARTSYS);
        /* we don't care about the exit status */
        he_message(1, "shell command failed");
      } else { /* child: pager program */
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);
        close(0);
        if (dup(pipe2[0]) != 0)
	  exit(EXIT_EXEC_FAILED);
        execvp(*pager, pager);
        exit(EXIT_EXEC_FAILED);
      }
    } else { /* !pager_f */
      if (write(pipe1[1], command, strlen(command)) != strlen(command) ||
	  write(pipe1[1], "\n", 1) != 1) {
	close(pipe1[1]);
	he_message(1, "couldn't send command `%s' to pager", command);
      } else {
      close(pipe1[1]);
      do if (waitpid(shell_pid, &shell_status, 0) >= 0) break;
        while (errno == ERESTARTSYS);
      if ((shell_x = WEXITSTATUS(shell_status))) {
        if (shell_x == EXIT_EXEC_FAILED)
          he_message(1, "couldn't start shell `%s'", *shell);
        else
          he_message(1, "%s exited with code %i", *shell, shell_x);
      }
      if (!shell_x) he_message(0, "shell command done");
      }
    }
  } else { /* child: shell */
    close(pipe1[1]);
    close(pipe2[0]);
    close(0);
    if (dup(pipe1[0]) != 0)
      exit(EXIT_EXEC_FAILED);
    if (pager_f) {
      close(1);
      if (dup(pipe2[1]) != 1)
	exit(EXIT_EXEC_FAILED);
      close(2);
      if (dup(pipe2[1]) != 2)
	exit(EXIT_EXEC_FAILED);
    }
    /* execute the shell */
    execvp(*shell, shell);
    exit(EXIT_EXEC_FAILED);
  }
#if 0
  if (!pager_f) {
    tio_last_line();
    tio_printf("\n@Ar press any key @~@r");
    tio_getch();
    tio_clear_to_eol();
  }
#else
  tio_last_line();
  tio_printf("\n@Ar press any key @~@r");
  tio_getch();
  tio_clear_to_eol();
#endif
exit_exh_shell_command:
  tio_restart();
  free(*shell);
  free(*pager);
  return 0;
}
