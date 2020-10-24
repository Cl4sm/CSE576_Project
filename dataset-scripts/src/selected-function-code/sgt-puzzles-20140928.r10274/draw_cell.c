static void draw_cell(drawing *draw, game_drawstate *ds, int r, int c,
                      drawcell cell)
{
    int const ts = ds->tilesize;
    int const y = BORDER + ts * r, x = BORDER + ts * c;
    int const tx = x + (ts / 2), ty = y + (ts / 2);
    int const dotsz = (ds->tilesize + 9) / 10;

    int const colour = (cell.value == BLACK ?
                        cell.error ? COL_ERROR : COL_BLACK :
                        cell.flash || cell.cursor ?
                        COL_LOWLIGHT : COL_BACKGROUND);

    draw_rect        (draw, x, y, ts, ts, colour);
    draw_rect_outline(draw, x, y, ts, ts, COL_GRID);

    switch (cell.value) {
      case WHITE: draw_rect(draw, tx - dotsz / 2, ty - dotsz / 2, dotsz, dotsz,
			    cell.error ? COL_ERROR : COL_USER);
      case BLACK: break;
      case EMPTY:
        if (cell.error)
            draw_circle(draw, tx, ty, dotsz / 2, COL_ERROR, COL_GRID);
        break;
      default:
	{
	    int const colour = (cell.error ? COL_ERROR : COL_GRID);
	    char *msg = nfmtstr(10, "%d", cell.value);
	    draw_text(draw, tx, ty, FONT_VARIABLE, ts * 3 / 5,
		      ALIGN_VCENTRE | ALIGN_HCENTRE, colour, msg);
	    sfree(msg);
	}
    }

    draw_update(draw, x, y, ts, ts);
}