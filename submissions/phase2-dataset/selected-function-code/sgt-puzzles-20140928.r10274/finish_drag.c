static char *finish_drag(const game_state *state, game_ui *ui)
{
    char buf[80];

    if (ui->dragx_src == -1 || ui->dragy_src == -1)
        return NULL;
    if (ui->dragx_dst == -1 || ui->dragy_dst == -1)
        return ui_cancel_drag(ui);

    if (ui->drag_is_noline) {
        sprintf(buf, "N%d,%d,%d,%d",
                ui->dragx_src, ui->dragy_src,
                ui->dragx_dst, ui->dragy_dst);
    } else {
        sprintf(buf, "L%d,%d,%d,%d,%d",
                ui->dragx_src, ui->dragy_src,
                ui->dragx_dst, ui->dragy_dst, ui->nlines);
    }

    ui_cancel_drag(ui);

    return dupstr(buf);
}