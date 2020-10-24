static int
ppc_reloc_size (int reloc_type)
{
  switch (reloc_type)
    {
    case R_PPC_ADDR16:
    case R_PPC_UADDR16:
    case R_PPC_ADDR16_LO:
    case R_PPC_ADDR16_HI:
    case R_PPC_ADDR16_HA:
    case R_PPC_DTPREL16:
    case R_PPC_DTPREL16_LO:
    case R_PPC_DTPREL16_HI:
    case R_PPC_DTPREL16_HA:
    case R_PPC_TPREL16:
    case R_PPC_TPREL16_LO:
    case R_PPC_TPREL16_HI:
    case R_PPC_TPREL16_HA:
      return 2;
    default:
      break;
    }
  return 4;
}
