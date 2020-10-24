void __attribute__((__sentinel__(0)))
radeon_show_reg_bits(const char *prefix, const char *name, unsigned long index_addr,
                     unsigned long data_addr, unsigned long addr, ...)
{
    va_list ap;
    int start, end;
    char *desc;
    int value;
    char format[32], intformat;

    if (index_addr && data_addr)
        value = radeon_get_indexed(index_addr, data_addr, addr, name);
    else
        value = radeon_get(addr, name);

    printf("%s%s\t%08x\n", prefix ? prefix : "", name, value);

    va_start(ap, addr);
    while (1) {
        start = va_arg(ap, int);
        end = va_arg(ap, int);
        desc = va_arg(ap, char *);

        if (!start && !end && !desc)
            break;

        if (strncmp(desc, "DECIMAL", 7) == 0) {
            desc += 7;
            intformat = 'd';
        }
        else {
            intformat = 'x';
        }
        /* FIXME There has to be a better way ... */
        sprintf(format, "\t%%s:\t%%%d%c\n", end - start + 1, intformat);
        printf(format, desc, (value >> start) & ((1 << (end - start + 1)) - 1));
    }
    va_end(ap);
}