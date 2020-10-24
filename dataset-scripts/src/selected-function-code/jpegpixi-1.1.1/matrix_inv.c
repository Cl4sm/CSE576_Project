static void
matrix_inv (double *const a, const size_t size)
{
    double *const b = xmalloc (sizeof (double) * size * size);
    size_t ix, iy, j;


    /* Copy matrix to new location.  */
    memcpy (b, a, sizeof (double) * size * size);

    /* Set destination matrix to unit matrix.  */
    for (iy = 0; iy < size; ++iy)
        for (ix = 0; ix < size; ++ix)
            a [iy * size + ix] = ix == iy ? 1.0 : 0.0;

    /* Convert matrix to upper triangle form.  */
    for (iy = 0; iy < size - 1; ++iy)
        for (j = iy + 1; j < size; ++j)
        {
            const double factor = b [j * size + iy] / b [iy * size + iy];

            for (ix = 0; ix < size; ++ix)
            {
                b [j * size + ix] -= factor * b [iy * size + ix];
                a [j * size + ix] -= factor * a [iy * size + ix];
            }
        }

    /* Convert matrix to diagonal form.  */
    for (iy = size - 1; iy > 0; --iy)
        for (j = 0; j < iy; ++j)
        {
            const double factor =  b [j * size + iy] / b [iy * size + iy];

            for (ix = 0; ix < size; ++ix)
                a [j * size + ix] -= factor * a [iy * size + ix];
        }

    /* Convert matrix to unit matrix.  */
    for (iy = 0; iy < size; ++iy)
        for (ix = 0; ix < size; ++ix)
            a [iy * size + ix] /= b [iy * size + iy];

    free (b);
}