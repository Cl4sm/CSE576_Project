static void atom_op_switch(atom_exec_context *ctx, int *ptr, int arg)
{
    uint8_t attr = U8((*ptr)++);
    uint32_t src, val, target;
    SDEBUG("   switch: ");
    src = atom_get_src(ctx, attr, ptr);
    while(U16(*ptr) != ATOM_CASE_END)
	if(U8(*ptr) == ATOM_CASE_MAGIC) {
	    (*ptr)++;
	    SDEBUG("   case: ");
	    val = atom_get_src(ctx, (attr&0x38)|ATOM_ARG_IMM, ptr);
	    target = U16(*ptr);
	    if(val == src) {
		SDEBUG("   target: %04X\n", target);
		*ptr = ctx->start+target;
		return;
	    }
	    (*ptr) += 2;
	} else {
	    printk(KERN_INFO "Bad case.\n");
	    return;
	}
    (*ptr) += 2;
}