static move *solver_reasoning_connectedness(game_state *state,
                                            int nclues,
                                            const square *clues,
                                            move *buf)
{
    int const w = state->params.w, h = state->params.h, n = w * h;

    square *const dfs_parent = snewn(n, square);
    int *const dfs_depth = snewn(n, int);

    int i;
    for (i = 0; i < n; ++i) {
        dfs_parent[i].r = NOT_VISITED;
        dfs_depth[i] = -n;
    }

    for (i = 0; i < n && state->grid[i] == BLACK; ++i);

    dfs_parent[i].r = i / w;
    dfs_parent[i].c = i % w; /* `dfs root`.parent == `dfs root` */
    dfs_depth[i] = 0;

    dfs_biconnect_visit(i / w, i % w, state, dfs_parent, dfs_depth, &buf);

    sfree(dfs_parent);
    sfree(dfs_depth);

    return buf;
}