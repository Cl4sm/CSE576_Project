int main(int argc, char *argv[])
{
  uint32_t s_attr = 0;		/* Attributes to set */
  uint32_t c_attr = 0;		/* Attributes to clear */
  int i;
  int files = 0;		/* Files processed */
  int e;

  program = argv[0];

  for ( i = 1 ; i < argc ; i++ ) {
    if ( argv[i][0] == '-' )
      c_attr |= parse_attr(argv[i]+1);
    else if ( argv[i][0] == '+' )
      s_attr |= parse_attr(argv[i]+1);
    else {
      e = 0;

      if ( c_attr | s_attr )
	e = chattr(argv[i], s_attr, c_attr);
      else
	e = lsattr(argv[i]);

      if ( e ) {
	perror(argv[i]);
	return EX_DATAERR;
      }

      files++;
    }
  }

  if ( !files )
    usage();

  return 0;
}