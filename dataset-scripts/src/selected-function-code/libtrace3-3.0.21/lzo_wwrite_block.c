static int lzo_wwrite_block(const char *buffer, off_t len, struct buffer_t *outbuf)
{
	char b2[MAX_BUFFER_SIZE];
	int err;
	lzo_uint dst_len;
	char scratch[LZO1X_1_MEM_COMPRESS];

	outbuf->offset=0;

	memset(scratch,0,sizeof(scratch));
	err=lzo1x_1_compress((void*)buffer, len, 
			(void*)b2, &dst_len, 
			scratch);

	switch(err) {
		case LZO_E_OK:
			break;
		case LZO_E_ERROR:
			return -EINVAL; /* WTF? */
		case LZO_E_OUT_OF_MEMORY:
			return -ENOMEM; /* Uh oh */
		case LZO_E_NOT_COMPRESSIBLE:
			return -EINVAL; /* Claimed not to be used, dunno what we'll do */
		case LZO_E_INPUT_OVERRUN:
			return -EINVAL;  /* Can't happen on compress? */
		case LZO_E_OUTPUT_OVERRUN:
			return -ENOMEM;
		case LZO_E_LOOKBEHIND_OVERRUN:
			return -EINVAL;
		case LZO_E_EOF_NOT_FOUND:
			return -ENOENT; /* Can't happen on compress? */
		case LZO_E_INPUT_NOT_CONSUMED:
			return -EINVAL;
		case LZO_E_NOT_YET_IMPLEMENTED:
			return -ENOSYS;
		default:
			fprintf(stderr,"Unknown lzo error %d\n",err);
			return -EINVAL;
	}

	write32(outbuf, len); /* Original length */
	write32(outbuf, min((uint32_t)len,(uint32_t)dst_len));
	/* CRC32 of the uncompressed buffer */
#if 0
	write32(outbuf, lzo_crc32(CRC32_INIT_VALUE, (void*)buffer, len));
#endif
	write32(outbuf, 
		lzo_adler32(ADLER32_INIT_VALUE, (const void*)buffer, len));
	write_buf(outbuf, b2, dst_len);

	/* Return the number of bytes compressed */
	return len;
}