static void
set_weighted_pixels (struct jfif *const jfif, const jpeg_component_info *const comp, const int icomp,
                     const int x_pos, const int y_pos, const int x_size, const int y_size,
                     const enum dimdir_t d, const int poly_order)
{
    const struct weight_s *w;
    int x, y;


    /* Obtain weight data block.  */
    {
        struct weight_s tmp;

        /* In the one-dimensional case, `x_size' must be 1, and the size must be stored in `y_size'.  */
        tmp.x_size = (d == twodim ? x_size : 1);
        tmp.y_size = (d == horizontal ? x_size : y_size);
        tmp.poly_order = poly_order;
        tmp.is_twodim = d == twodim;

        w = rbtree_find (&weight_s_tree, &tmp, weight_s_cmp);

        if (w == 0)
        {
            struct weight_s *const new_w = get_weights (tmp.x_size, tmp.y_size, tmp.is_twodim, poly_order);

            if ((w = rbtree_insert (&weight_s_tree, new_w, sizeof *new_w, weight_s_cmp, 0)) == 0)
                mem_alloc_failed ();
            free (new_w);
        }
    }


    /* Calculate weighted pixel sum.  */
    for (y = 0; y < y_size; ++y)
        for (x = 0; x < x_size; ++x)
            if (XY_VALID (x_pos + x, y_pos + y))
            {
                const double *const weights = w->weights [d == vertical ? y : d == horizontal ? x : x + x_size * y];
                double sum_weight = 0.0, v = 0.0;
                size_t i;

                for (i = 0; i < w->num_pos; ++i)
                {
                    /* In the one-dimensional case, only the y coordinate is used.  */
                    const int xx = x_pos + (d == vertical   ? x : d == horizontal ? w->pos [i].y : w->pos [i].x);
                    const int yy = y_pos + (d == horizontal ? y : w->pos [i].y);

                    if (XY_VALID (xx, yy))
                    {
                        const double weight = weights [i];

                        v += weight * get_pixel (jfif, icomp, xx, yy);
                        sum_weight += weight;
                    }
                }

                if (fabs (v) <= DBL_MIN)
                    set_pixel (jfif, icomp, x_pos + x, y_pos + y, 0.0);
                else if (sum_weight >= DBL_MIN)
                    set_pixel (jfif, icomp, x_pos + x, y_pos + y, v / sum_weight);
                else if (v >= 0.0)
                    set_pixel (jfif, icomp, x_pos + x, y_pos + y, DBL_MAX);
                else
                    set_pixel (jfif, icomp, x_pos + x, y_pos + y, -DBL_MAX);
            }
}