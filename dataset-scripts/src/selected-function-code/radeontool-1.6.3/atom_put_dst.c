static void atom_put_dst(atom_exec_context *ctx, int arg, uint8_t attr, int *ptr, uint32_t val, uint32_t saved)
{
    uint32_t align = atom_dst_to_src[(attr>>3)&7][(attr>>6)&3], old_val = val, idx;
    struct atom_context *gctx = ctx->ctx;
    old_val &= atom_arg_mask[align] >> atom_arg_shift[align];
    val <<= atom_arg_shift[align];
    val &= atom_arg_mask[align];
    saved &= ~atom_arg_mask[align];
    val |= saved;
    switch(arg) {
    case ATOM_ARG_REG:
	idx = U16(*ptr);
	(*ptr)+=2;
	DEBUG("REG[0x%04X]", idx);
	idx += gctx->reg_block;
	switch(gctx->io_mode) {
	case ATOM_IO_MM:
	    if(idx == 0)
		gctx->card->reg_write(gctx->card, idx, val<<2);
	    else
		gctx->card->reg_write(gctx->card, idx, val);
	    break;
	case ATOM_IO_PCI:
	    printk(KERN_INFO "PCI registers are not implemented.\n");
	    return;
	case ATOM_IO_SYSIO:
	    printk(KERN_INFO "SYSIO registers are not implemented.\n");
	    return;
	default:
	    if(!(gctx->io_mode&0x80)) {
		printk(KERN_INFO "Bad IO mode.\n");
		return;
	    }
	    if(!gctx->iio[gctx->io_mode&0xFF]) {
		printk(KERN_INFO "Undefined indirect IO write method %d.\n", gctx->io_mode&0x7F);
		return;
	    }
	    atom_iio_execute(gctx, gctx->iio[gctx->io_mode&0xFF], idx, val);
	}
	break;
    case ATOM_ARG_PS:
	idx = U8(*ptr);
	(*ptr)++;
	DEBUG("PS[0x%02X]", idx);
	ctx->ps[idx] = val;
	break;
    case ATOM_ARG_WS:
	idx = U8(*ptr);
	(*ptr)++;
	DEBUG("WS[0x%02X]", idx);
	switch(idx) {
	case ATOM_WS_QUOTIENT:
	    gctx->divmul[0] = val;
	    break;
	case ATOM_WS_REMAINDER:
	    gctx->divmul[1] = val;
	    break;
	case ATOM_WS_DATAPTR:
	    gctx->data_block = val;
	    break;
	case ATOM_WS_SHIFT:
	    gctx->shift = val;
	    break;
	case ATOM_WS_OR_MASK:
	case ATOM_WS_AND_MASK:
	    break;
	case ATOM_WS_FB_WINDOW:
	    gctx->fb_base = val;
	    break;
	case ATOM_WS_ATTRIBUTES:
	    gctx->io_attr = val;
	    break;
	default:
	    ctx->ws[idx] = val;
	}
	break;
    case ATOM_ARG_FB:
	idx = U8(*ptr);
	(*ptr)++;
	DEBUG("FB[0x%02X]", idx);
	*(uint32_t *)(gctx->atom_fb_scratch + gctx->fb_base + idx) = val;
	//	printk(KERN_INFO "FB access is not implemented.\n");
	return;
    case ATOM_ARG_PLL:
	idx = U8(*ptr);
	(*ptr)++;
	DEBUG("PLL[0x%02X]", idx);
	gctx->card->reg_write(gctx->card, PLL_INDEX, idx);
	gctx->card->reg_write(gctx->card, PLL_DATA, val);
	break;
    case ATOM_ARG_MC:
	idx = U8(*ptr);
	(*ptr)++;
	DEBUG("MC[0x%02X]", idx);
	gctx->card->mc_write(gctx->card, idx, val);
	return;
    }
    switch(align) {
    case ATOM_SRC_DWORD:
	DEBUG(".[31:0] <- 0x%08X\n", old_val);
	break;
    case ATOM_SRC_WORD0:
	DEBUG(".[15:0] <- 0x%04X\n", old_val);
	break;
    case ATOM_SRC_WORD8:
	DEBUG(".[23:8] <- 0x%04X\n", old_val);
	break;
    case ATOM_SRC_WORD16:
	DEBUG(".[31:16] <- 0x%04X\n", old_val);
	break;
    case ATOM_SRC_BYTE0:
	DEBUG(".[7:0] <- 0x%02X\n", old_val);
	break;
    case ATOM_SRC_BYTE8:
	DEBUG(".[15:8] <- 0x%02X\n", old_val);
	break;
    case ATOM_SRC_BYTE16:
	DEBUG(".[23:16] <- 0x%02X\n", old_val);
	break;
    case ATOM_SRC_BYTE24:
	DEBUG(".[31:24] <- 0x%02X\n", old_val);
	break;
    }
}