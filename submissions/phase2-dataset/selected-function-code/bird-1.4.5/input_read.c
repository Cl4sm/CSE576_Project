void
input_read(void)
{
  char buf[INPUT_BUF_LEN];

  if ((fgets(buf, INPUT_BUF_LEN, stdin) == NULL) || (buf[0] == 0))
  {
    putchar('\n');
    cleanup();
    exit(0);
  }

  int l = strlen(buf);
  if ((l+1) == INPUT_BUF_LEN)
    {
      printf("Input too long.\n");
      return;
    }

  if (buf[l-1] == '\n')
    buf[--l] = '\0';

  if (!interactive)
    printf("%s\n", buf);

  if (l == 0)
    return;

  if (lastnb(buf, l) == '?')
    {
      cmd_help(buf, strlen(buf));
      return;
    }

  submit_command(buf);
}
