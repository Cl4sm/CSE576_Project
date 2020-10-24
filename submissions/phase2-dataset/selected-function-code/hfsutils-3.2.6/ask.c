int ask(char *question, ...)
{
  int result = -1;
  va_list args;

  va_start(args, question);

  vprintf(question, args);

  if (! REPAIR)
    {
      printf(".\n");
      result = 0;
    }
  else if (YES)
    {
      printf(": fixing.\n");
      result = 1;
    }
  else
    {
      while (1)
	{
	  char answer[80];

	  if (feof(stdin))
	    {
	      printf("...\n");
	      result = 0;
	      break;
	    }

	  printf(". Fix? ");
	  fflush(stdout);

	  fgets(answer, sizeof(answer), stdin);

	  if (feof(stdin))
	    {
	      printf("\n");
	      result = 0;
	      break;
	    }

	  switch (answer[0])
	    {
	    case 'y':
	    case 'Y':
	      result = 1;
	      break;

	    case 'n':
	    case 'N':
	      result = 0;
	      break;
	    }

	  if (result == -1)
	    vprintf(question, args);
	  else
	    break;
	}
    }

  va_end(args);

  return result;
}
