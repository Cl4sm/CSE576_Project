mdebug_sym_relocate (unsigned int st, unsigned int sc)
{
  switch (sc)
    {
    case scData:
    case scBss:
    case scAbs:
    case scSData:
    case scSBss:
    case scRData:
    case scXData:
    case scPData:
      return 1;
    case scText:
    case scInit:
    case scFini:
    case scRConst:
      if (st != stBlock && st != stEnd && st != stFile)
	return 1;
    default:
      return 0;
    }
}
