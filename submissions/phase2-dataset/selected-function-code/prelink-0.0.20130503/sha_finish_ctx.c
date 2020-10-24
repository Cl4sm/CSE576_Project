void *
sha_finish_ctx (struct sha_ctx *ctx, void *resbuf)
{
  /* Take yet unprocessed bytes into account.  */
  md5_uint32 bytes = ctx->buflen;
  size_t pad;

  /* Now count remaining bytes.  */
  ctx->total[0] += bytes;
  if (ctx->total[0] < bytes)
    ++ctx->total[1];

  pad = bytes >= 56 ? 64 + 56 - bytes : 56 - bytes;
  memcpy (&ctx->buffer[bytes], fillbuf, pad);

  /* Put the 64-bit file length in *bits* at the end of the buffer.  */
  *(md5_uint32 *) &ctx->buffer[bytes + pad + 4] = NOTSWAP (ctx->total[0] << 3);
  *(md5_uint32 *) &ctx->buffer[bytes + pad] = NOTSWAP ((ctx->total[1] << 3) |
						    (ctx->total[0] >> 29));

  /* Process last bytes.  */
  sha_process_block (ctx->buffer, bytes + pad + 8, ctx);

  return sha_read_ctx (ctx, resbuf);
}
