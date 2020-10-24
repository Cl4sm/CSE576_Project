struct atom_context *atom_parse(struct card_info *card, void *bios)
{
    int base;
    struct atom_context *ctx = kzalloc(sizeof(struct atom_context), GFP_KERNEL);
    char *str;

    ctx->card = card;
    ctx->bios = bios;

    if(CU16(0) != ATOM_BIOS_MAGIC) {
	printk(KERN_INFO "Invalid BIOS magic.\n");
	kfree(ctx);
	return NULL;
    }
    if(strncmp(CSTR(ATOM_ATI_MAGIC_PTR), ATOM_ATI_MAGIC, strlen(ATOM_ATI_MAGIC))) {
	printk(KERN_INFO "Invalid ATI magic.\n");
	kfree(ctx);
	return NULL;
    }

    base = CU16(ATOM_ROM_TABLE_PTR);
    if(strncmp(CSTR(base+ATOM_ROM_MAGIC_PTR), ATOM_ROM_MAGIC, strlen(ATOM_ROM_MAGIC))) {
	printk(KERN_INFO "Invalid ATOM magic.\n");
	kfree(ctx);
	return NULL;
    }

    ctx->atom_fb_scratch=kzalloc(20*1024, GFP_KERNEL);
    ctx->cmd_table = CU16(base+ATOM_ROM_CMD_PTR);
    ctx->data_table = CU16(base+ATOM_ROM_DATA_PTR);
    atom_index_iio(ctx, CU16(ctx->data_table+ATOM_DATA_IIO_PTR)+4);

    str = CSTR(CU16(base+ATOM_ROM_MSG_PTR));
    while(*str && ((*str == '\n') || (*str == '\r')))
    	str++;
    printk(KERN_INFO "ATOM BIOS: %s", str);

    return ctx;
}