int
objectSetChunks(ObjectPtr object, int numchunks)
{
    int n;

    if(numchunks <= object->numchunks)
        return 0;

    if(object->length >= 0)
        n = MAX(numchunks, (object->length + (CHUNK_SIZE - 1)) / CHUNK_SIZE);
    else
        n = MAX(numchunks, 
                MAX(object->numchunks + 2, object->numchunks * 5 / 4));
    
    if(n == 0) {
        assert(object->chunks == NULL);
    } else if(object->numchunks == 0) {
        object->chunks = calloc(n, sizeof(ChunkRec));
        if(object->chunks == NULL) {
            return -1;
        }
        object->numchunks = n;
    } else {
        ChunkPtr newchunks;
        newchunks = realloc(object->chunks, n * sizeof(ChunkRec));
        if(newchunks == NULL)
            return -1;
        memset(newchunks + object->numchunks, 0,
               (n - object->numchunks) * sizeof(ChunkRec));
        object->chunks = newchunks;
        object->numchunks = n;
    }
    return 0;
}