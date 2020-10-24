static void text_add_line(struct TextData *td, const char *begin, const char *end)
{
    int idx = td->text->count;
    char *out;
    if (idx == td->reserved) {
        if (td->reserved == 0) td->reserved = 1;
        td->reserved *= 2;
        td->text->lines = realloc(td->text->lines, td->reserved * sizeof td->text->lines[0]);
    }

    out = td->text->lines[idx] = malloc(end - begin + 1);
    for (; begin != end; ++begin, ++out) {
        switch (*begin) {
            case '\n':
            case '\t':
            case '\r':
            case '#': *out = ' '; break;
            default: *out = *begin; break;
        }
    }
    *out = '\0';

    td->text->count++;
}