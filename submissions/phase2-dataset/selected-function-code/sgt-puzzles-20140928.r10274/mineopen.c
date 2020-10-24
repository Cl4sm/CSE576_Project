static int mineopen(void *vctx, int x, int y)
{
    struct minectx *ctx = (struct minectx *)vctx;
    int i, j, n;

    assert(x >= 0 && x < ctx->w && y >= 0 && y < ctx->h);
    if (ctx->grid[y * ctx->w + x])
	return -1;		       /* *bang* */

    n = 0;
    for (i = -1; i <= +1; i++) {
	if (x + i < 0 || x + i >= ctx->w)
	    continue;
	for (j = -1; j <= +1; j++) {
	    if (y + j < 0 || y + j >= ctx->h)
		continue;
	    if (i == 0 && j == 0)
		continue;
	    if (ctx->grid[(y+j) * ctx->w + (x+i)])
		n++;
	}
    }

    return n;
}