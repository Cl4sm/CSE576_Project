static void unruly_draw_tile(drawing *dr, int x, int y, int tilesize, int tile)
{
    clip(dr, x, y, tilesize, tilesize);

    /* Draw the grid edge first, so the tile can overwrite it */
    draw_rect(dr, x, y, tilesize, tilesize, COL_GRID);

    /* Background of the tile */
    {
        int val = (tile & FF_ZERO ? 0 : tile & FF_ONE ? 2 : 1);
        val = (val == 0 ? COL_0 : val == 2 ? COL_1 : COL_EMPTY);

        if ((tile & (FF_FLASH1 | FF_FLASH2)) &&
             (val == COL_0 || val == COL_1)) {
            val += (tile & FF_FLASH1 ? 1 : 2);
        }

        draw_rect(dr, x, y, tilesize-1, tilesize-1, val);

        if ((val == COL_0 || val == COL_1) && (tile & FF_IMMUTABLE)) {
            draw_rect(dr, x + tilesize/6, y + tilesize/6,
                      tilesize - 2*(tilesize/6) - 2, 1, val + 2);
            draw_rect(dr, x + tilesize/6, y + tilesize/6,
                      1, tilesize - 2*(tilesize/6) - 2, val + 2);
            draw_rect(dr, x + tilesize/6 + 1, y + tilesize - tilesize/6 - 2,
                      tilesize - 2*(tilesize/6) - 2, 1, val + 1);
            draw_rect(dr, x + tilesize - tilesize/6 - 2, y + tilesize/6 + 1,
                      1, tilesize - 2*(tilesize/6) - 2, val + 1);
        }
    }

    /* 3-in-a-row errors */
    if (tile & (FE_HOR_ROW_LEFT | FE_HOR_ROW_RIGHT)) {
        int left = x, right = x + tilesize - 1;
        if ((tile & FE_HOR_ROW_LEFT))
            right += tilesize/2;
        if ((tile & FE_HOR_ROW_RIGHT))
            left -= tilesize/2;
        unruly_draw_err_rectangle(dr, left, y, right-left, tilesize-1, tilesize);
    }
    if (tile & (FE_VER_ROW_TOP | FE_VER_ROW_BOTTOM)) {
        int top = y, bottom = y + tilesize - 1;
        if ((tile & FE_VER_ROW_TOP))
            bottom += tilesize/2;
        if ((tile & FE_VER_ROW_BOTTOM))
            top -= tilesize/2;
        unruly_draw_err_rectangle(dr, x, top, tilesize-1, bottom-top, tilesize);
    }

    /* Count errors */
    if (tile & FE_COUNT) {
        draw_text(dr, x + tilesize/2, y + tilesize/2, FONT_VARIABLE,
                  tilesize/2, ALIGN_HCENTRE | ALIGN_VCENTRE, COL_ERROR, "!");
    }

    /* Row-match errors */
    if (tile & FE_ROW_MATCH) {
        draw_rect(dr, x, y+tilesize/2-tilesize/12,
                  tilesize, 2*(tilesize/12), COL_ERROR);
    }
    if (tile & FE_COL_MATCH) {
        draw_rect(dr, x+tilesize/2-tilesize/12, y,
                  2*(tilesize/12), tilesize, COL_ERROR);
    }

    /* Cursor rectangle */
    if (tile & FF_CURSOR) {
        draw_rect(dr, x, y, tilesize/12, tilesize-1, COL_CURSOR);
        draw_rect(dr, x, y, tilesize-1, tilesize/12, COL_CURSOR);
        draw_rect(dr, x+tilesize-1-tilesize/12, y, tilesize/12, tilesize-1,
                  COL_CURSOR);
        draw_rect(dr, x, y+tilesize-1-tilesize/12, tilesize-1, tilesize/12,
                  COL_CURSOR);
    }

    unclip(dr);
    draw_update(dr, x, y, tilesize, tilesize);
}