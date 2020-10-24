void MD5Final(unsigned char digest[16], MD5Context *pCtx){
        struct Context *ctx = (struct Context *)pCtx;
	unsigned count;
	unsigned char *p;

	/* Compute number of bytes mod 64 */
	count = (ctx->bits[0] >> 3) & 0x3F;

	/* Set the first char of padding to 0x80.  This is safe since there is
	   always at least one byte free */
	p = ctx->in + count;
	*p++ = 0x80;

	/* Bytes of padding needed to make 64 bytes */
	count = 64 - 1 - count;

	/* Pad out to 56 mod 64 */
	if (count < 8) {
		/* Two lots of padding:  Pad the first block to 64 bytes */
		memset(p, 0, count);
		byteReverse(ctx->in, 16);
		MD5Transform(ctx->buf, (uint32 *)ctx->in);

		/* Now fill the next block with 56 bytes */
		memset(ctx->in, 0, 56);
	} else {
		/* Pad block to 56 bytes */
		memset(p, 0, count-8);
	}
	byteReverse(ctx->in, 14);

	/* Append length in bits and transform */
	((uint32 *)ctx->in)[ 14 ] = ctx->bits[0];
	((uint32 *)ctx->in)[ 15 ] = ctx->bits[1];

	MD5Transform(ctx->buf, (uint32 *)ctx->in);
	byteReverse((unsigned char *)ctx->buf, 4);
	memcpy(digest, ctx->buf, 16);
	memset(ctx, 0, sizeof(ctx));	/* In case it's sensitive */
}