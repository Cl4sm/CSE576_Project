void
more(void)
{
  more_begin();
  printf("--More--\015");
  fflush(stdout);

 redo:
  switch (getchar())
    {
    case ' ':
      num_lines = 2;
      break;
    case '\n':
    case '\r':
      num_lines--;
      break;
    case 'q':
      skip_input = 1;
      break;
    default:
      goto redo;
    }

  printf("        \015");
  fflush(stdout);
  more_end();
}
