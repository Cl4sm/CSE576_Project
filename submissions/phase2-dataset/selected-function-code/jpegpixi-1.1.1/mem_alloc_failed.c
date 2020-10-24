void
mem_alloc_failed (void)
{
    {
        const char *ptr = invocation_name;
        size_t len = strlen (ptr);

        while (len > 0)
        {
            const ssize_t chars_written = write (STDERR_FILENO, ptr, len);
            /* If writing to standard error fails, terminate right away.  */
            if (chars_written == -1)
                exit (1);
            len -= (size_t)chars_written;
            ptr += (ptrdiff_t)chars_written;
        }
    }

    {
        static const char message [] = ": out of memory\n";
        const char *ptr = message;
        size_t len = numof (message) - 1;

        while (len > 0)
        {
            const ssize_t chars_written = write (STDERR_FILENO, ptr, len);
            /* If writing to standard error fails, terminate right away.  */
            if (chars_written == -1)
                exit (1);
            len -= (size_t)chars_written;
            ptr += (ptrdiff_t)chars_written;
        }
    }

    exit (1);
}