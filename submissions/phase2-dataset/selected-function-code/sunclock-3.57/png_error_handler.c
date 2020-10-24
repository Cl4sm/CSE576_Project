png_error_handler (png_structp png_ptr, png_const_charp msg)
{
    jmpbuf_wrapper  *jmpbuf_ptr;

    /* this function, aside from the extra step of retrieving the "error
     * pointer" (below) and the fact that it exists within the application
     * rather than within libpng, is essentially identical to libpng's
     * default error handler.  The second point is critical:  since both
     * setjmp() and longjmp() are called from the same code, they are
     * guaranteed to have compatible notions of how big a jmp_buf is,
     * regardless of whether _BSD_SOURCE or anything else has (or has not)
     * been defined. */

    fprintf(stderr, "Fatal libpng error !!\n");
    fflush(stderr);

    jmpbuf_ptr = png_get_error_ptr(png_ptr);
    if (jmpbuf_ptr == NULL) {         /* we are completely hosed now */
        exit(99);
    }

    longjmp(jmpbuf_ptr->jmpbuf, 1);
}
