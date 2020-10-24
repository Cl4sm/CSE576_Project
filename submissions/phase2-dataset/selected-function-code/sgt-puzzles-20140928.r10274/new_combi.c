combi_ctx *new_combi(int r, int n)
{
    long nfr, nrf;
    combi_ctx *combi;

    assert(r <= n);
    assert(n >= 1);

    combi = snew(combi_ctx);
    memset(combi, 0, sizeof(combi_ctx));
    combi->r = r;
    combi->n = n;

    combi->a = snewn(r, int);
    memset(combi->a, 0, r * sizeof(int));

    nfr = factx(n, r+1);
    nrf = factx(n-r, 1);
    combi->total = (int)(nfr / nrf);

    reset_combi(combi);
    return combi;
}