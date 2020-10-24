Text* create_text( struct _Font *fnt, const char *orig_str, int width )
{
    struct TextData td;
    const char *line_start = orig_str;
    const char *head = orig_str;
    const char *committed = orig_str;
    int cumulated_width = 0;	/* width of text in this line */
    int break_line = 0;
    if (width < 0) width = 0;

    memset(&td, 0, sizeof(td));
    td.text = calloc ( 1, sizeof( Text ) );

    while (*committed) {
        int ch_width = char_width(fnt, *head);

        if (committed != head && text_is_linebreak(head[-1]))
            break_line = 1;
        else if (cumulated_width > width) {
            /* if the word is too long to fit into one line,
             * force a line break at the current position.
             */
            if (committed == line_start)
                /* take away last char (unless only one) and break there */
                committed = head - (head - 1 != line_start);
            head = committed;
            break_line = 1;
        }
        else if (text_is_breakable(committed, head))
            committed = head;

        if (!break_line) {
            cumulated_width += ch_width;
            head++;
        }

        if (!*head) break_line = 1;

        if (break_line) {
            text_add_line(&td, line_start, head);
            line_start = committed = head;
            cumulated_width = 0;
            break_line = 0;
        }
    }

    if (!td.text->lines) text_add_line(&td, "", "" + 1);

    return td.text;
}