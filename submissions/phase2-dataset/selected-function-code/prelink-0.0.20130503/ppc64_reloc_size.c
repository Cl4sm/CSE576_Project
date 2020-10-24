static int
ppc64_reloc_size (int reloc_type)
{
  switch (reloc_type)
    {
    case R_PPC64_ADDR16:
    case R_PPC64_UADDR16:
    case R_PPC64_ADDR16_LO:
    case R_PPC64_ADDR16_HA:
    case R_PPC64_ADDR16_HI:
    case R_PPC64_ADDR16_LO_DS:
    case R_PPC64_ADDR16_DS:
    case R_PPC64_ADDR16_HIGHER:
    case R_PPC64_ADDR16_HIGHERA:
    case R_PPC64_ADDR16_HIGHEST:
    case R_PPC64_ADDR16_HIGHESTA:
    case R_PPC64_DTPREL16:
    case R_PPC64_DTPREL16_LO:
    case R_PPC64_DTPREL16_HI:
    case R_PPC64_DTPREL16_HA:
    case R_PPC64_TPREL16:
    case R_PPC64_TPREL16_LO:
    case R_PPC64_TPREL16_HI:
    case R_PPC64_TPREL16_HA:
      return 2;
    case R_PPC64_GLOB_DAT:
    case R_PPC64_ADDR64:
    case R_PPC64_UADDR64:
    case R_PPC64_REL64:
    case R_PPC64_DTPMOD64:
    case R_PPC64_DTPREL64:
    case R_PPC64_TPREL64:
    case R_PPC64_IRELATIVE:
      return 8;
    default:
      break;
    }
  return 4;
}
