static int dfs_biconnect_visit(puzzle_size r, puzzle_size c,
                               game_state *state,
                               square *dfs_parent, int *dfs_depth,
                               move **buf)
{
    const puzzle_size w = state->params.w, h = state->params.h;
    int const i = idx(r, c, w), mydepth = dfs_depth[i];
    int lowpoint = mydepth, j, nchildren = 0;

    for (j = 0; j < 4; ++j) {
        const puzzle_size rr = r + dr[j], cc = c + dc[j];
        int const cell = idx(rr, cc, w);

        if (out_of_bounds(rr, cc, w, h)) continue;
        if (state->grid[cell] == BLACK) continue;

        if (dfs_parent[cell].r == NOT_VISITED) {
            int child_lowpoint;
            dfs_parent[cell].r = r;
            dfs_parent[cell].c = c;
            dfs_depth[cell] = mydepth + 1;
            child_lowpoint = dfs_biconnect_visit(rr, cc, state, dfs_parent,
                                                 dfs_depth, buf);

            if (child_lowpoint >= mydepth && mydepth > 0)
                solver_makemove(r, c, M_WHITE, state, buf);

            lowpoint = min(lowpoint, child_lowpoint);
            ++nchildren;
        } else if (rr != dfs_parent[i].r || cc != dfs_parent[i].c) {
            lowpoint = min(lowpoint, dfs_depth[cell]);
        }
    }

    if (mydepth == 0 && nchildren >= 2)
        solver_makemove(r, c, M_WHITE, state, buf);

    return lowpoint;
}