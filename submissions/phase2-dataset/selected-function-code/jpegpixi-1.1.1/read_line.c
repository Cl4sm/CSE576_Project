static char *
read_line (FILE *const f, const char *const filename, char **const buffer_ptr, size_t *const len_buffer_ptr,
           unsigned int *const lineno_ptr)
{
    size_t i = 0;
    int line_nonempty = 0, no_comment = 1;

    ++*lineno_ptr;

    while (1)
    {
#if HAVE_FGETC_UNLOCKED
        const int c = fgetc_unlocked (f);
#else
        const int c = fgetc (f);
#endif

        if (i >= *len_buffer_ptr)
            *buffer_ptr = xrealloc (*buffer_ptr, *len_buffer_ptr *= 2);

        if (c == EOF)
        {
            if (ferror (f))
            {
                fprintf (stderr, _("%s: %s: %s\n"), invocation_name, filename, strerror (errno));
                exit (1);
            }

            (*buffer_ptr) [i] = '\0';
            return line_nonempty ? *buffer_ptr : 0;
        }
        else if (c == '\n')
        {
            if (line_nonempty)
            {
                (*buffer_ptr) [i] = '\0';
                return *buffer_ptr;
            }
            else
            {
                i = 0;
                no_comment = 1;
                ++*lineno_ptr;
            }
        }
        else if (c == '#')
            no_comment = 0;
        else if (no_comment && c != '\0')
        {
            (*buffer_ptr) [i++] = c;
            if (!isspace (c))
                line_nonempty = 1;
        }
    }
}