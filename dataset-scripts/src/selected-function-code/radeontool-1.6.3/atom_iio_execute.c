static uint32_t atom_iio_execute(struct atom_context *ctx, int base, uint32_t index, uint32_t data)
{
    uint32_t temp = 0xCDCDCDCD;
    while(1)
	switch(CU8(base)) {
	case ATOM_IIO_NOP:
	    base++;
	    break;
	case ATOM_IIO_READ:
	    temp = ctx->card->reg_read(ctx->card, CU16(base+1));
	    base+=3;
	    break;
	case ATOM_IIO_WRITE:
	    ctx->card->reg_write(ctx->card, CU16(base+1), temp);
	    base+=3;
	    break;
	case ATOM_IIO_CLEAR:
	    temp &= ~((0xFFFFFFFF >> (32-CU8(base+1))) << CU8(base+2));
	    base+=3;
	    break;
	case ATOM_IIO_SET:
	    temp |= (0xFFFFFFFF >> (32-CU8(base+1))) << CU8(base+2);
	    base+=3;
	    break;
	case ATOM_IIO_MOVE_INDEX:
	    temp &= ~((0xFFFFFFFF >> (32-CU8(base+1))) << CU8(base+2));
	    temp |= ((index >> CU8(base+2)) & (0xFFFFFFFF >> (32-CU8(base+1)))) << CU8(base+3);
	    base+=4;
	    break;
	case ATOM_IIO_MOVE_DATA:
	    temp &= ~((0xFFFFFFFF >> (32-CU8(base+1))) << CU8(base+2));
	    temp |= ((data >> CU8(base+2)) & (0xFFFFFFFF >> (32-CU8(base+1)))) << CU8(base+3);
	    base+=4;
	    break;
	case ATOM_IIO_MOVE_ATTR:
	    temp &= ~((0xFFFFFFFF >> (32-CU8(base+1))) << CU8(base+2));
	    temp |= ((ctx->io_attr >> CU8(base+2)) & (0xFFFFFFFF >> (32-CU8(base+1)))) << CU8(base+3);
	    base+=4;
	    break;
	case ATOM_IIO_END:
	    return temp;
	default:
	    printk(KERN_INFO "Unknown IIO opcode.\n");
	    return 0;
	}
}