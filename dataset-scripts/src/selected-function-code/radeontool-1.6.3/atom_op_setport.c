static void atom_op_setport(atom_exec_context *ctx, int *ptr, int arg)
{
    int port;
    switch(arg) {
    case ATOM_PORT_ATI:
	port = U16(*ptr);
	if(port < ATOM_IO_NAMES_CNT)
		SDEBUG("   port: %d (%s)\n", port, atom_io_names[port]);
	else
		SDEBUG("   port: %d\n", port);
	if(!port)
	    ctx->ctx->io_mode = ATOM_IO_MM;
	else
	    ctx->ctx->io_mode = ATOM_IO_IIO|port;
	(*ptr)+=2;
	break;
    case ATOM_PORT_PCI:
	ctx->ctx->io_mode = ATOM_IO_PCI;
	(*ptr)++;
	break;
    case ATOM_PORT_SYSIO:
	ctx->ctx->io_mode = ATOM_IO_SYSIO;
	(*ptr)++;
	break;
    }
}