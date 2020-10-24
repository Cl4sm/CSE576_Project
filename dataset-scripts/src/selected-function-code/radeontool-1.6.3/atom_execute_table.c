void atom_execute_table(struct atom_context *ctx, int index, uint32_t *params)
{
    int base = CU16(ctx->cmd_table+4+2*index);
    int len, ws, ps, ptr;
    unsigned char op;
    atom_exec_context ectx;

    if(!base)
    	return;

    len = CU16(base+ATOM_CT_SIZE_PTR);
    ws = CU8(base+ATOM_CT_WS_PTR);
    ps = CU8(base+ATOM_CT_PS_PTR) & ATOM_CT_PS_MASK;
    ptr = base+ATOM_CT_CODE_PTR;

    SDEBUG(">> execute %04X (len %d, WS %d, PS %d)\n", base, len, ws, ps);

    /* reset reg block */
    ctx->reg_block = 0;
    ectx.ctx = ctx;
    ectx.ps_shift = ps/4;
    ectx.start = base;
    ectx.ps = params;
    if(ws)
 	ectx.ws = kzalloc(4*ws, GFP_KERNEL);
    else
	ectx.ws = NULL;

    debug_depth++;
    while(1) {
	op = CU8(ptr++);
	if(op<ATOM_OP_NAMES_CNT)
	    SDEBUG("%s @ 0x%04X\n", atom_op_names[op], ptr-1);
	else
	    SDEBUG("[%d] @ 0x%04X\n", op, ptr-1);

	if(op<ATOM_OP_CNT && op>0)
	    opcode_table[op].func(&ectx, &ptr, opcode_table[op].arg);
	else
	    break;

	if(op == ATOM_OP_EOT)
	    break;
    }
    debug_depth--;
    SDEBUG("<<\n");

    if(ws)
	kfree(ectx.ws);
}