static int set_faces(penrose_state *state, vector *vs, int n, int depth)
{
    setface_ctx *sf_ctx = (setface_ctx *)state->ctx;
    int i;
    int xs[4], ys[4];

    if (depth < state->max_depth) return 0;
#ifdef DEBUG_PENROSE
    if (n != 4) return 0; /* triangles are sent as debugging. */
#endif

    for (i = 0; i < n; i++) {
        double tx = v_x(vs, i), ty = v_y(vs, i);

        xs[i] = (int)round_int_nearest_away(tx);
        ys[i] = (int)round_int_nearest_away(ty);

        if (xs[i] < sf_ctx->xmin || xs[i] > sf_ctx->xmax) return 0;
        if (ys[i] < sf_ctx->ymin || ys[i] > sf_ctx->ymax) return 0;
    }

    grid_face_add_new(sf_ctx->g, n);
    debug(("penrose: new face l=%f gen=%d...",
           penrose_side_length(state->start_size, depth), depth));
    for (i = 0; i < n; i++) {
        grid_dot *d = grid_get_dot(sf_ctx->g, sf_ctx->points,
                                   xs[i], ys[i]);
        grid_face_set_dot(sf_ctx->g, d, i);
        debug((" ... dot 0x%x (%d,%d) (was %2.2f,%2.2f)",
               d, d->x, d->y, v_x(vs, i), v_y(vs, i)));
    }

    return 0;
}