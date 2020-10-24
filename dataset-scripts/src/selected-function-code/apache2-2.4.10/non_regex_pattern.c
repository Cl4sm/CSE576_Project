static const char *non_regex_pattern(apr_pool_t *p, const char *s)
{
    const char *src = s;
    int escapes_found = 0;
    int in_escape = 0;

    while (*src) {
        switch (*src) {
        case '^':
        case '.':
        case '$':
        case '|':
        case '(':
        case ')':
        case '[':
        case ']':
        case '*':
        case '+':
        case '?':
        case '{':
        case '}':
            if (!in_escape) {
                return NULL;
            }
            in_escape = 0;
            break;
        case '\\':
            if (!in_escape) {
                in_escape = 1;
                escapes_found = 1;
            }
            else {
                in_escape = 0;
            }
            break;
        default:
            if (in_escape) {
                return NULL;
            }
            break;
        }
        src++;
    }
    if (!escapes_found) {
        return s;
    }
    else {
        char *unescaped = (char *)apr_palloc(p, src - s + 1);
        char *dst = unescaped;
        src = s;
        do {
            if (*src == '\\') {
                src++;
            }
        } while ((*dst++ = *src++));
        return unescaped;
    }
}