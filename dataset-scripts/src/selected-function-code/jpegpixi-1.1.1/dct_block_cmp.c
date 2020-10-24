static int
dct_block_cmp (const void *const a, const void *const b)
{
    const struct dct_block *const dct_block_a = a;
    const struct dct_block *const dct_block_b = b;

    if (dct_block_a->icomp < dct_block_b->icomp)
        return -1;
    else if (dct_block_a->icomp > dct_block_b->icomp)
        return 1;
    else if (dct_block_a->x < dct_block_b->x)
        return -1;
    else if (dct_block_a->x > dct_block_b->x)
        return 1;
    else if (dct_block_a->y < dct_block_b->y)
        return -1;
    else if (dct_block_a->y > dct_block_b->y)
        return 1;
    else
        return 0;
}