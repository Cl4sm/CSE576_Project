static int
weight_s_cmp (const void *const a, const void *const b)
{
    const struct weight_s *const weight_s_a = a;
    const struct weight_s *const weight_s_b = b;

    if (weight_s_a->is_twodim < weight_s_b->is_twodim)
        return -1;
    else if (weight_s_a->is_twodim > weight_s_b->is_twodim)
        return 1;
    else if (weight_s_a->poly_order < weight_s_b->poly_order)
        return -1;
    else if (weight_s_a->poly_order > weight_s_b->poly_order)
        return 1;
    else if (weight_s_a->x_size < weight_s_b->x_size)
        return -1;
    else if (weight_s_a->x_size > weight_s_b->x_size)
        return 1;
    else if (weight_s_a->y_size < weight_s_b->y_size)
        return -1;
    else if (weight_s_a->y_size > weight_s_b->y_size)
        return 1;
    else
        return 0;
}