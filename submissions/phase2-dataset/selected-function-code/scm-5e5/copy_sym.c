copy_sym (new, a_out, a_name, new_name)
     int new, a_out;
     char *a_name, *new_name;
{
  char page[1024];
  int n;

  if (a_out < 0)
    return 0;

#ifdef COFF
  if (SYMS_START == 0L)
    return 0;
#endif  /* COFF */

#ifdef COFF
  if (lnnoptr)			/* if there is line number info */
    lseek (a_out, lnnoptr, 0);	/* start copying from there */
  else
#endif /* COFF */
    lseek (a_out, SYMS_START, 0);	/* Position a.out to symtab. */

  while ((n = read (a_out, page, sizeof page)) > 0)
    {
      if (write (new, page, n) != n)
	{
	  PERROR (new_name);
	}
    }
  if (n < 0)
    {
      PERROR (a_name);
    }
  return 0;
}
