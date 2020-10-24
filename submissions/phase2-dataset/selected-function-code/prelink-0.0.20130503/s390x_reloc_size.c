static int
s390x_reloc_size (int reloc_type)
{
  switch (reloc_type)
    {
    case R_390_GLOB_DAT:
    case R_390_JMP_SLOT:
    case R_390_64:
    case R_390_PC64:
    case R_390_IRELATIVE:
      return 8;
    case R_390_32:
    case R_390_PC32:
    case R_390_PC32DBL:
    case R_390_PLT32DBL:
    default:
      return 4;
    case R_390_16:
    case R_390_PC16:
    case R_390_PC16DBL:
    case R_390_PLT16DBL:
      return 2;
    case R_390_8:
      return 1;
    }
}
