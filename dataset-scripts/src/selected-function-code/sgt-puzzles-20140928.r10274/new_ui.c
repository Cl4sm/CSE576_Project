static game_ui *new_ui(const game_state *state)
{
    game_ui *ui = snew(game_ui);
    int x, y, v;

    ui->sx = ui->sy = ui->dx = ui->dy = 0;
    ui->dragging = FALSE;
    ui->cur_visible = ui->cur_jumping = 0;

    /* make sure we start the cursor somewhere on the grid. */
    for (x = 0; x < state->w; x++) {
        for (y = 0; y < state->h; y++) {
            v = state->grid[y*state->w+x];
            if (v == GRID_PEG || v == GRID_HOLE) {
                ui->cur_x = x; ui->cur_y = y;
                goto found;
            }
        }
    }
    assert(!"new_ui found nowhere for cursor");
found:

    return ui;
}