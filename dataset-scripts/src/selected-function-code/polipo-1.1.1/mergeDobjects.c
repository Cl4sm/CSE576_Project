static void
mergeDobjects(DiskObjectPtr dst, DiskObjectPtr src)
{
    if(dst->filename == NULL) {
        dst->filename = src->filename;
        dst->body_offset = src->body_offset;
    } else
        free(src->filename);
    free(src->location);
    if(dst->length < 0)
        dst->length = src->length;
    if(dst->size < 0)
        dst->size = src->size;
    if(dst->age < 0)
        dst->age = src->age;
    if(dst->date < 0)
        dst->date = src->date;
    if(dst->last_modified < 0)
        dst->last_modified = src->last_modified;
    free(src);
}