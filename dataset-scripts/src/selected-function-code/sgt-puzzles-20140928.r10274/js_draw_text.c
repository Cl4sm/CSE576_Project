static void js_draw_text(void *handle, int x, int y, int fonttype,
                         int fontsize, int align, int colour, char *text)
{
    char fontstyle[80];
    int halign;

    sprintf(fontstyle, "%dpx %s", fontsize,
            fonttype == FONT_FIXED ? "monospace" : "sans-serif");

    if (align & ALIGN_VCENTRE)
	y += js_canvas_find_font_midpoint(fontsize, fontstyle);

    if (align & ALIGN_HCENTRE)
	halign = 1;
    else if (align & ALIGN_HRIGHT)
        halign = 2;
    else
        halign = 0;

    js_canvas_draw_text(x, y, halign, colour_strings[colour], fontstyle, text);
}