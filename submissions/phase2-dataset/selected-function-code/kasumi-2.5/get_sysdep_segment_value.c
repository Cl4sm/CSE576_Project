static const char *
get_sysdep_segment_value (name)
     const char *name;
{
  /* Test for an ISO C 99 section 7.8.1 format string directive.
     Syntax:
     P R I { d | i | o | u | x | X }
     { { | LEAST | FAST } { 8 | 16 | 32 | 64 } | MAX | PTR }  */
  /* We don't use a table of 14 times 6 'const char *' strings here, because
     data relocations cost startup time.  */
  if (name[0] == 'P' && name[1] == 'R' && name[2] == 'I')
    {
      if (name[3] == 'd' || name[3] == 'i' || name[3] == 'o' || name[3] == 'u'
	  || name[3] == 'x' || name[3] == 'X')
	{
	  if (name[4] == '8' && name[5] == '\0')
	    {
	      if (name[3] == 'd')
		return PRId8;
	      if (name[3] == 'i')
		return PRIi8;
	      if (name[3] == 'o')
		return PRIo8;
	      if (name[3] == 'u')
		return PRIu8;
	      if (name[3] == 'x')
		return PRIx8;
	      if (name[3] == 'X')
		return PRIX8;
	      abort ();
	    }
	  if (name[4] == '1' && name[5] == '6' && name[6] == '\0')
	    {
	      if (name[3] == 'd')
		return PRId16;
	      if (name[3] == 'i')
		return PRIi16;
	      if (name[3] == 'o')
		return PRIo16;
	      if (name[3] == 'u')
		return PRIu16;
	      if (name[3] == 'x')
		return PRIx16;
	      if (name[3] == 'X')
		return PRIX16;
	      abort ();
	    }
	  if (name[4] == '3' && name[5] == '2' && name[6] == '\0')
	    {
	      if (name[3] == 'd')
		return PRId32;
	      if (name[3] == 'i')
		return PRIi32;
	      if (name[3] == 'o')
		return PRIo32;
	      if (name[3] == 'u')
		return PRIu32;
	      if (name[3] == 'x')
		return PRIx32;
	      if (name[3] == 'X')
		return PRIX32;
	      abort ();
	    }
	  if (name[4] == '6' && name[5] == '4' && name[6] == '\0')
	    {
	      if (name[3] == 'd')
		return PRId64;
	      if (name[3] == 'i')
		return PRIi64;
	      if (name[3] == 'o')
		return PRIo64;
	      if (name[3] == 'u')
		return PRIu64;
	      if (name[3] == 'x')
		return PRIx64;
	      if (name[3] == 'X')
		return PRIX64;
	      abort ();
	    }
	  if (name[4] == 'L' && name[5] == 'E' && name[6] == 'A'
	      && name[7] == 'S' && name[8] == 'T')
	    {
	      if (name[9] == '8' && name[10] == '\0')
		{
		  if (name[3] == 'd')
		    return PRIdLEAST8;
		  if (name[3] == 'i')
		    return PRIiLEAST8;
		  if (name[3] == 'o')
		    return PRIoLEAST8;
		  if (name[3] == 'u')
		    return PRIuLEAST8;
		  if (name[3] == 'x')
		    return PRIxLEAST8;
		  if (name[3] == 'X')
		    return PRIXLEAST8;
		  abort ();
		}
	      if (name[9] == '1' && name[10] == '6' && name[11] == '\0')
		{
		  if (name[3] == 'd')
		    return PRIdLEAST16;
		  if (name[3] == 'i')
		    return PRIiLEAST16;
		  if (name[3] == 'o')
		    return PRIoLEAST16;
		  if (name[3] == 'u')
		    return PRIuLEAST16;
		  if (name[3] == 'x')
		    return PRIxLEAST16;
		  if (name[3] == 'X')
		    return PRIXLEAST16;
		  abort ();
		}
	      if (name[9] == '3' && name[10] == '2' && name[11] == '\0')
		{
		  if (name[3] == 'd')
		    return PRIdLEAST32;
		  if (name[3] == 'i')
		    return PRIiLEAST32;
		  if (name[3] == 'o')
		    return PRIoLEAST32;
		  if (name[3] == 'u')
		    return PRIuLEAST32;
		  if (name[3] == 'x')
		    return PRIxLEAST32;
		  if (name[3] == 'X')
		    return PRIXLEAST32;
		  abort ();
		}
	      if (name[9] == '6' && name[10] == '4' && name[11] == '\0')
		{
		  if (name[3] == 'd')
		    return PRIdLEAST64;
		  if (name[3] == 'i')
		    return PRIiLEAST64;
		  if (name[3] == 'o')
		    return PRIoLEAST64;
		  if (name[3] == 'u')
		    return PRIuLEAST64;
		  if (name[3] == 'x')
		    return PRIxLEAST64;
		  if (name[3] == 'X')
		    return PRIXLEAST64;
		  abort ();
		}
	    }
	  if (name[4] == 'F' && name[5] == 'A' && name[6] == 'S'
	      && name[7] == 'T')
	    {
	      if (name[8] == '8' && name[9] == '\0')
		{
		  if (name[3] == 'd')
		    return PRIdFAST8;
		  if (name[3] == 'i')
		    return PRIiFAST8;
		  if (name[3] == 'o')
		    return PRIoFAST8;
		  if (name[3] == 'u')
		    return PRIuFAST8;
		  if (name[3] == 'x')
		    return PRIxFAST8;
		  if (name[3] == 'X')
		    return PRIXFAST8;
		  abort ();
		}
	      if (name[8] == '1' && name[9] == '6' && name[10] == '\0')
		{
		  if (name[3] == 'd')
		    return PRIdFAST16;
		  if (name[3] == 'i')
		    return PRIiFAST16;
		  if (name[3] == 'o')
		    return PRIoFAST16;
		  if (name[3] == 'u')
		    return PRIuFAST16;
		  if (name[3] == 'x')
		    return PRIxFAST16;
		  if (name[3] == 'X')
		    return PRIXFAST16;
		  abort ();
		}
	      if (name[8] == '3' && name[9] == '2' && name[10] == '\0')
		{
		  if (name[3] == 'd')
		    return PRIdFAST32;
		  if (name[3] == 'i')
		    return PRIiFAST32;
		  if (name[3] == 'o')
		    return PRIoFAST32;
		  if (name[3] == 'u')
		    return PRIuFAST32;
		  if (name[3] == 'x')
		    return PRIxFAST32;
		  if (name[3] == 'X')
		    return PRIXFAST32;
		  abort ();
		}
	      if (name[8] == '6' && name[9] == '4' && name[10] == '\0')
		{
		  if (name[3] == 'd')
		    return PRIdFAST64;
		  if (name[3] == 'i')
		    return PRIiFAST64;
		  if (name[3] == 'o')
		    return PRIoFAST64;
		  if (name[3] == 'u')
		    return PRIuFAST64;
		  if (name[3] == 'x')
		    return PRIxFAST64;
		  if (name[3] == 'X')
		    return PRIXFAST64;
		  abort ();
		}
	    }
	  if (name[4] == 'M' && name[5] == 'A' && name[6] == 'X'
	      && name[7] == '\0')
	    {
	      if (name[3] == 'd')
		return PRIdMAX;
	      if (name[3] == 'i')
		return PRIiMAX;
	      if (name[3] == 'o')
		return PRIoMAX;
	      if (name[3] == 'u')
		return PRIuMAX;
	      if (name[3] == 'x')
		return PRIxMAX;
	      if (name[3] == 'X')
		return PRIXMAX;
	      abort ();
	    }
	  if (name[4] == 'P' && name[5] == 'T' && name[6] == 'R'
	      && name[7] == '\0')
	    {
	      if (name[3] == 'd')
		return PRIdPTR;
	      if (name[3] == 'i')
		return PRIiPTR;
	      if (name[3] == 'o')
		return PRIoPTR;
	      if (name[3] == 'u')
		return PRIuPTR;
	      if (name[3] == 'x')
		return PRIxPTR;
	      if (name[3] == 'X')
		return PRIXPTR;
	      abort ();
	    }
	}
    }
  /* Other system dependent strings are not valid.  */
  return NULL;
}
