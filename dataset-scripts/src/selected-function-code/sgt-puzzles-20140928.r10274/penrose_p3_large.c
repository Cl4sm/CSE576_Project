static int penrose_p3_large(penrose_state *state, int depth, int flip,
                            vector v_orig, vector v_edge)
{
    vector vv_orig;

#ifdef DEBUG_PENROSE
    {
        vector vs[3];
        vs[0] = v_orig;
        XFORM(1, 0, 1, 0);
        XFORM(2, 0, 0, -36*flip);

        state->new_tile(state, vs, 3, depth);
    }
#endif

    if (flip > 0) {
        vector vs[4];

        vs[0] = v_orig;
        XFORM(1, 0, 0, -36);
        XFORM(2, 0, 1, 0);
        XFORM(3, 0, 0, 36);

        state->new_tile(state, vs, 4, depth);
    }
    if (depth >= state->max_depth) return 0;

    vv_orig = v_trans(v_orig, v_edge);

    penrose_p3_large(state, depth+1, -flip,
                     vv_orig, v_shrinkphi(v_rotate(v_edge, 180)));

    penrose_p3_small(state, depth+1, flip,
                     vv_orig, v_shrinkphi(v_rotate(v_edge, -108*flip)));

    vv_orig = v_trans(v_orig, v_growphi(v_edge));

    penrose_p3_large(state, depth+1, flip,
                     vv_orig, v_shrinkphi(v_rotate(v_edge, -144*flip)));


    return 0;
}