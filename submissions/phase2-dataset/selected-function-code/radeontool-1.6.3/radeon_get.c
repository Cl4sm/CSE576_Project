static unsigned int radeon_get(unsigned long offset, const char *name)
{
    unsigned int value;

    if (debug) 
        printf("reading %s (%lx) is ", name, offset);

    if (ctrl_mem == NULL)
        die("internal error");

#ifdef __powerpc__
    __asm__ __volatile__ ("lwbrx %0,%1,%2\n\t"
                          "eieio"
                          : "=r" (value)
                          : "b" (ctrl_mem), "r"(offset),
                          "m" (*((volatile unsigned int *)ctrl_mem+offset)));
#else
    value = *(unsigned int * volatile)(ctrl_mem + offset);
#endif

    if (debug) 
        printf("%08x\n", value);

    return value;
}