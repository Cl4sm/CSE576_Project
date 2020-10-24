static struct dct_block *
get_block (struct jfif *const jfif, const int icomp, const int x_block, const int y_block)
{
    struct dct_block tmp, *block;


    /* Test if the previously requested block is requested again (very common).  */
    if (prev_dct_block != 0 && prev_dct_block->icomp == icomp
        && prev_dct_block->x == x_block && prev_dct_block->y == y_block)
        return prev_dct_block;

    /* Try to find DCT block.  */
    tmp.icomp = icomp;
    tmp.x = x_block;
    tmp.y = y_block;

    block = rbtree_find (&jfif->dct_block_tree, &tmp, dct_block_cmp);

    /* DCT block does not yet exist.  Create it and store it in the tree.  */
    if (block == 0)
    {
        const jpeg_component_info *const comp = &(jfif->jpg->comp_info [icomp]);
        const JBLOCKROW row = (jfif->jpg->mem->access_virt_barray) ((j_common_ptr)jfif->jpg, jfif->coeff [icomp],
                                                                    (JDIMENSION)y_block, (JDIMENSION)1, TRUE) [0];
        const UINT16 *const quant = jfif->jpg->quant_tbl_ptrs [comp->quant_tbl_no]->quantval;

        tmp.coeff = row [x_block];
        tmp.quant = quant;
        idct (tmp.values, row [x_block], quant);
        tmp.dirty = 0;

        if ((block = rbtree_insert (&jfif->dct_block_tree, &tmp, sizeof tmp, dct_block_cmp, 0)) == 0)
            mem_alloc_failed ();
    }

    prev_dct_block = block;
    return block;
}