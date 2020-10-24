static void atom_op_jump(atom_exec_context *ctx, int *ptr, int arg)
{
    int execute = 0, target = U16(*ptr);
    (*ptr)+=2;
    switch(arg) {
    case ATOM_COND_ABOVE:
	execute = ctx->ctx->cs_above;
	break;
    case ATOM_COND_ABOVEOREQUAL:
	execute = ctx->ctx->cs_above || ctx->ctx->cs_equal;
	break;
    case ATOM_COND_ALWAYS:
	execute = 1;
	break;
    case ATOM_COND_BELOW:
	execute = !(ctx->ctx->cs_above || ctx->ctx->cs_equal);
	break;
    case ATOM_COND_BELOWOREQUAL:
	execute = !ctx->ctx->cs_above;
	break;
    case ATOM_COND_EQUAL:
	execute = ctx->ctx->cs_equal;
	break;
    case ATOM_COND_NOTEQUAL:
	execute = !ctx->ctx->cs_equal;
	break;
    }
    if(arg != ATOM_COND_ALWAYS)
	SDEBUG("   taken: %s\n", execute?"yes":"no");
    SDEBUG("   target: 0x%04X\n", target);
    if(execute)
	*ptr = ctx->start+target;
}