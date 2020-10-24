struct weight_s *
get_weights (const int x_size, const int y_size, const int is_twodim, const int poly_order)
{
    struct weight_s *w;
    const size_t num_coeff = (is_twodim
                              ? ((size_t)poly_order + 1) * ((size_t)poly_order + 1)
                              : (size_t)poly_order + 1);
    double *matrix;  /* num_coeff * num_coeff */
    double *vector0; /* num_pos   * num_coeff */
    double *vector1; /* num_pos   * num_coeff */
    size_t ix, iy, j;
    int x, y;


    w = xmalloc (sizeof (struct weight_s));
    w->x_size = x_size;
    w->y_size = y_size;
    w->poly_order = poly_order;
    w->is_twodim = is_twodim;


    /* Determine coordinates of pixels to be sampled.  */
    w->num_pos = 0;

    if (is_twodim)
    {
        size_t len_pos = 8;

        w->pos = xmalloc (sizeof (struct xy_s) * len_pos);

        for (y = -poly_order; y < y_size + poly_order; ++y)
            for (x = -poly_order; x < x_size + poly_order; ++x)
                if ((x < 0 && y < 0 && -x - y < poly_order + 2)
                    || (x < 0 && y >= y_size && -x + y - y_size < poly_order + 1)
                    || (x >= x_size && y < 0 && x - y - x_size < poly_order + 1)
                    || (x >= x_size && y >= y_size && x + y - x_size - y_size < poly_order)
                    || (x < 0 && y >= 0 && y < y_size) || (x >= x_size && y >= 0 && y < y_size)
                    || (y < 0 && x >= 0 && x < x_size) || (y >= y_size && x >= 0 && x < x_size))
                {
                    if (w->num_pos >= len_pos)
                        w->pos = xrealloc (w->pos, sizeof (struct xy_s) * (len_pos *= 2));

                    w->pos [w->num_pos].x = x;
                    w->pos [w->num_pos++].y = y;
                }

        w->pos = xrealloc (w->pos, sizeof (struct xy_s) * w->num_pos);
    }
    else
    {
        /* In the one-dimensional case, only the y coordinate and y size is used.  */
        w->pos = xmalloc (sizeof (struct xy_s) * 2 * poly_order);

        for (y = -poly_order; y < 0; ++y)
        {
            w->pos [w->num_pos].x = 0;
            w->pos [w->num_pos++].y = y;
        }

        for (y = y_size; y < y_size + poly_order; ++y)
        {
            w->pos [w->num_pos].x = 0;
            w->pos [w->num_pos++].y = y;
        }
    }


    /* Allocate memory.  */
    matrix = xmalloc (sizeof (double) * num_coeff * num_coeff);
    vector0 = xmalloc (sizeof (double) * w->num_pos * num_coeff);
    vector1 = xmalloc (sizeof (double) * w->num_pos * num_coeff);


    /* Calculate coefficient matrix and vector.  */
    for (iy = 0; iy < num_coeff; ++iy)
    {
        for (ix = 0; ix < num_coeff; ++ix)
            matrix [iy * num_coeff + ix] = 0.0;

        for (j = 0; j < w->num_pos; ++j)
        {
            vector0 [iy * w->num_pos + j] = poly_term (iy, w->pos [j].x, w->pos [j].y, poly_order);

            for (ix = 0; ix < num_coeff; ++ix)
                matrix [iy * num_coeff + ix] += (vector0 [iy * w->num_pos + j]
                                                 * poly_term (ix, w->pos [j].x, w->pos [j].y, poly_order));
        }
    }


    /* Invert matrix.  */
    matrix_inv (matrix, num_coeff);


    /* Multiply inverse matrix with vector.  */
    for (iy = 0; iy < num_coeff; ++iy)
        for (j = 0; j < w->num_pos; ++j)
        {
            vector1 [iy * w->num_pos + j] = 0.0;

            for (ix = 0; ix < num_coeff; ++ix)
                vector1 [iy * w->num_pos + j] += matrix [iy * num_coeff + ix] * vector0 [ix * w->num_pos + j];
        }


    /* Store weights.  */
    w->weights = xmalloc (sizeof (double *) * x_size * y_size);

    for (y = 0; y < y_size; ++y)
        for (x = 0; x < x_size; ++x)
        {
            double *const weights = xmalloc (sizeof (double) * w->num_pos);
            w->weights [x + x_size * y] = weights;

            for (j = 0; j < w->num_pos; ++j)
            {
                weights [j] = 0.0;

                for (iy = 0; iy < num_coeff; ++iy)
                    weights [j] += vector1 [iy * w->num_pos + j] * poly_term (iy, x, y, poly_order);

                weights [j] *= (double)w->num_pos;
            }
        }


    free (vector1);
    free (vector0);
    free (matrix);

    return w;
}