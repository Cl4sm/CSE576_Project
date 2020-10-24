static char *update_drag_dst(const game_state *state, game_ui *ui,
                             const game_drawstate *ds, int nx, int ny)
{
    int ox, oy, dx, dy, i, currl, maxb;
    struct island *is;
    grid_type gtype, ntype, mtype, curr;

    if (ui->dragx_src == -1 || ui->dragy_src == -1) return NULL;

    ui->dragx_dst = -1;
    ui->dragy_dst = -1;

    /* work out which of the four directions we're closest to... */
    ox = COORD(ui->dragx_src) + TILE_SIZE/2;
    oy = COORD(ui->dragy_src) + TILE_SIZE/2;

    if (abs(nx-ox) < abs(ny-oy)) {
        dx = 0;
        dy = (ny-oy) < 0 ? -1 : 1;
        gtype = G_LINEV; ntype = G_NOLINEV; mtype = G_MARKV;
        maxb = INDEX(state, maxv, ui->dragx_src+dx, ui->dragy_src+dy);
    } else {
        dy = 0;
        dx = (nx-ox) < 0 ? -1 : 1;
        gtype = G_LINEH; ntype = G_NOLINEH; mtype = G_MARKH;
        maxb = INDEX(state, maxh, ui->dragx_src+dx, ui->dragy_src+dy);
    }
    if (ui->drag_is_noline) {
        ui->todraw = ntype;
    } else {
        curr = GRID(state, ui->dragx_src+dx, ui->dragy_src+dy);
        currl = INDEX(state, lines, ui->dragx_src+dx, ui->dragy_src+dy);

        if (curr & gtype) {
            if (currl == maxb) {
                ui->todraw = 0;
                ui->nlines = 0;
            } else {
                ui->todraw = gtype;
                ui->nlines = currl + 1;
            }
        } else {
            ui->todraw = gtype;
            ui->nlines = 1;
        }
    }

    /* ... and see if there's an island off in that direction. */
    is = INDEX(state, gridi, ui->dragx_src, ui->dragy_src);
    for (i = 0; i < is->adj.npoints; i++) {
        if (is->adj.points[i].off == 0) continue;
        curr = GRID(state, is->x+dx, is->y+dy);
        if (curr & mtype) continue; /* don't allow changes to marked lines. */
        if (ui->drag_is_noline) {
            if (curr & gtype) continue; /* no no-line where already a line */
        } else {
            if (POSSIBLES(state, dx, is->x+dx, is->y+dy) == 0) continue; /* no line if !possible. */
            if (curr & ntype) continue; /* can't have a bridge where there's a no-line. */
        }

        if (is->adj.points[i].dx == dx &&
            is->adj.points[i].dy == dy) {
            ui->dragx_dst = ISLAND_ORTHX(is,i);
            ui->dragy_dst = ISLAND_ORTHY(is,i);
        }
    }
    /*debug(("update_drag src (%d,%d) d(%d,%d) dst (%d,%d)\n",
           ui->dragx_src, ui->dragy_src, dx, dy,
           ui->dragx_dst, ui->dragy_dst));*/
    return "";
}