static int
sparc64_reloc_size (int reloc_type)
{
  switch (reloc_type)
    {
    case R_SPARC_8:
    case R_SPARC_DISP8:
      return 1;
    case R_SPARC_16:
    case R_SPARC_DISP16:
    case R_SPARC_UA16:
      return 2;
    case R_SPARC_RELATIVE:
    case R_SPARC_64:
    case R_SPARC_UA64:
    case R_SPARC_GLOB_DAT:
      return 8;
    default:
      break;
    }
  return 4;
}
