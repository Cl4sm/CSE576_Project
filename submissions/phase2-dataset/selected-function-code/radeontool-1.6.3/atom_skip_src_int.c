static void atom_skip_src_int(atom_exec_context *ctx, uint8_t attr, int *ptr)
{
    uint32_t align = (attr >> 3) & 7, arg = attr & 7;
    switch(arg) {
    case ATOM_ARG_REG:
    case ATOM_ARG_ID:
	(*ptr)+=2;
	break;
    case ATOM_ARG_PLL:
    case ATOM_ARG_MC:
    case ATOM_ARG_PS:
    case ATOM_ARG_WS:
    case ATOM_ARG_FB:
	(*ptr)++;
	break;
    case ATOM_ARG_IMM:
	switch(align) {
	case ATOM_SRC_DWORD:
	    (*ptr)+=4;
	    return;
	case ATOM_SRC_WORD0:
	case ATOM_SRC_WORD8:
	case ATOM_SRC_WORD16:
	    (*ptr)+=2;
	    return;
	case ATOM_SRC_BYTE0:
	case ATOM_SRC_BYTE8:
	case ATOM_SRC_BYTE16:
	case ATOM_SRC_BYTE24:
	    (*ptr)++;
	    return;
	}
	return;
    }
}