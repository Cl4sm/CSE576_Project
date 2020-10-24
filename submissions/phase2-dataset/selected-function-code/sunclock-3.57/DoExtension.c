DoExtension(FILE *fd, int label)
{
    static char buf[256];
    char *str;

    switch (label) {
    case 0x01:			/* Plain Text Extension */
	str = "Plain Text Extension";
	break;
    case 0xff:			/* Application Extension */
	str = "Application Extension";
	break;
    case 0xfe:			/* Comment Extension */
	str = "Comment Extension";
	while (GetDataBlock(fd, (unsigned char *) buf) != 0);
	return FALSE;
    case 0xf9:			/* Graphic Control Extension */
	str = "Graphic Control Extension";
	(void) GetDataBlock(fd, (unsigned char *) buf);
	Gif89.disposal = (buf[0] >> 2) & 0x7;
	Gif89.inputFlag = (buf[0] >> 1) & 0x1;
	Gif89.delayTime = LM_to_uint(buf[1], buf[2]);
	if ((buf[0] & 0x1) != 0)
	    Gif89.transparent = buf[3];

	while (GetDataBlock(fd, (unsigned char *) buf) != 0);
	return FALSE;
    default:
	str = buf;
	sprintf(buf, "UNKNOWN (0x%02x)", label);
	break;
    }

    while (GetDataBlock(fd, (unsigned char *) buf) != 0);

    return FALSE;
}
