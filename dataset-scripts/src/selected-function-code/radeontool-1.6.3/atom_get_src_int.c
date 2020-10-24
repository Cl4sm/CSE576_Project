static uint32_t atom_get_src_int(atom_exec_context *ctx, uint8_t attr, int *ptr, uint32_t *saved, int print)
{
    uint32_t idx, val = 0xCDCDCDCD, align, arg;
    struct atom_context *gctx = ctx->ctx;
    arg = attr & 7;
    align = (attr >> 3) & 7;
    switch(arg) {
    case ATOM_ARG_REG:
	idx = U16(*ptr);
	(*ptr)+=2;
	if(print)
	    DEBUG("REG[0x%04X]", idx);
	idx += gctx->reg_block;
	switch(gctx->io_mode) {
	case ATOM_IO_MM:
	    val = gctx->card->reg_read(gctx->card, idx);
	    break;
	case ATOM_IO_PCI:
	    printk(KERN_INFO "PCI registers are not implemented.\n");
	    return 0;
	case ATOM_IO_SYSIO:
	    printk(KERN_INFO "SYSIO registers are not implemented.\n");
	    return 0;
	default:
	    if(!(gctx->io_mode&0x80)) {
		printk(KERN_INFO "Bad IO mode.\n");
		return 0;
	    }
	    if(!gctx->iio[gctx->io_mode&0x7F]) {
		printk(KERN_INFO "Undefined indirect IO read method %d.\n", gctx->io_mode&0x7F);
		return 0;
	    }
	    val = atom_iio_execute(gctx, gctx->iio[gctx->io_mode&0x7F], idx, 0);
	}
	break;
    case ATOM_ARG_PS:
	idx = U8(*ptr);
	(*ptr)++;
	val = ctx->ps[idx];
	if(print)
	    DEBUG("PS[0x%02X,0x%04X]", idx, val);
	break;
    case ATOM_ARG_WS:
	idx = U8(*ptr);
	(*ptr)++;
	if(print)
	    DEBUG("WS[0x%02X]", idx);
	switch(idx) {
	case ATOM_WS_QUOTIENT:
	    val = gctx->divmul[0];
	    break;
	case ATOM_WS_REMAINDER:
	    val = gctx->divmul[1];
	    break;
	case ATOM_WS_DATAPTR:
	    val = gctx->data_block;
	    break;
	case ATOM_WS_SHIFT:
	    val = gctx->shift;
	    break;
	case ATOM_WS_OR_MASK:
	    val = 1<<gctx->shift;
	    break;
	case ATOM_WS_AND_MASK:
	    val = ~(1<<gctx->shift);
	    break;
	case ATOM_WS_FB_WINDOW:
	    val = gctx->fb_base;
	    break;
	case ATOM_WS_ATTRIBUTES:
	    val = gctx->io_attr;
	    break;
	default:
	    val = ctx->ws[idx];
	}
	break;
    case ATOM_ARG_ID:
	idx = U16(*ptr);
	(*ptr)+=2;
	if(print) {
	    if(gctx->data_block)
		DEBUG("ID[0x%04X+%04X]", idx, gctx->data_block);
	    else
		DEBUG("ID[0x%04X]", idx);
	}
	val = U32(idx + gctx->data_block);
	break;
    case ATOM_ARG_FB:
	idx = U8(*ptr);
	(*ptr)++;
	if(print)
	    DEBUG("FB[0x%02X]", idx);
	val = *(uint32_t*)(gctx->atom_fb_scratch + gctx->fb_base + idx);
	//	printk(KERN_INFO "FB access is not implemented.\n");
	return val;
    case ATOM_ARG_IMM:
	switch(align) {
	case ATOM_SRC_DWORD:
	    val = U32(*ptr);
	    (*ptr)+=4;
	    if(print)
		DEBUG("IMM 0x%08X\n", val);
	    return val;
	case ATOM_SRC_WORD0:
	case ATOM_SRC_WORD8:
	case ATOM_SRC_WORD16:
	    val = U16(*ptr);
	    (*ptr)+=2;
	    if(print)
		DEBUG("IMM 0x%04X\n", val);
	    return val;
	case ATOM_SRC_BYTE0:
	case ATOM_SRC_BYTE8:
	case ATOM_SRC_BYTE16:
	case ATOM_SRC_BYTE24:
	    val = U8(*ptr);
	    (*ptr)++;
	    if(print)
		DEBUG("IMM 0x%02X\n", val);
	    return val;
	}
	return 0;
    case ATOM_ARG_PLL:
	idx = U8(*ptr);
	(*ptr)++;
	if(print)
	    DEBUG("PLL[0x%02X]", idx);
	gctx->card->reg_write(gctx->card, PLL_INDEX, idx);
	val = gctx->card->reg_read(gctx->card, PLL_DATA);
	break;
    case ATOM_ARG_MC:
	idx = U8(*ptr);
	(*ptr)++;
	if(print)
	    DEBUG("MC[0x%02X]", idx);
	val = gctx->card->mc_read(gctx->card, idx);
	return 0;
    }
    if(saved)
	*saved = val;
    val &= atom_arg_mask[align];
    val >>= atom_arg_shift[align];
    if(print)
	switch(align) {
	case ATOM_SRC_DWORD:
	    DEBUG(".[31:0] -> 0x%08X\n", val);
	    break;
	case ATOM_SRC_WORD0:
	    DEBUG(".[15:0] -> 0x%04X\n", val);
	    break;
	case ATOM_SRC_WORD8:
	    DEBUG(".[23:8] -> 0x%04X\n", val);
	    break;
	case ATOM_SRC_WORD16:
	    DEBUG(".[31:16] -> 0x%04X\n", val);
	    break;
	case ATOM_SRC_BYTE0:
	    DEBUG(".[7:0] -> 0x%02X\n", val);
	    break;
	case ATOM_SRC_BYTE8:
	    DEBUG(".[15:8] -> 0x%02X\n", val);
	    break;
	case ATOM_SRC_BYTE16:
	    DEBUG(".[23:16] -> 0x%02X\n", val);
	    break;
	case ATOM_SRC_BYTE24:
	    DEBUG(".[31:24] -> 0x%02X\n", val);
	    break;
	}
    return val;
}