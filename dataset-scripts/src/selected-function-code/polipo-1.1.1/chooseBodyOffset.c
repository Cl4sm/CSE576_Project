static int
chooseBodyOffset(int n, ObjectPtr object)
{
    int length = MAX(object->size, object->length);
    int body_offset;

    if(object->length >= 0 && object->length + n < 4096 - 4)
        return -1;              /* no gap for small objects */

    if(n <= 128)
        body_offset = 256;
    else if(n <= 192)
        body_offset = 384;
    else if(n <= 256)
        body_offset = 512;
    else if(n <= 384)
        body_offset = 768;
    else if(n <= 512)
        body_offset = 1024;
    else if(n <= 1024)
        body_offset = 2048;
    else if(n < 2048)
        body_offset = 4096;
    else
        body_offset = ((n + 32 + 4095) / 4096 + 1) * 4096;

    /* Tweak the gap so that we don't use up a full disk block for
       a small tail */
    if(object->length >= 0 && object->length < 64 * 1024) {
        int last = (body_offset + object->length) % 4096;
        int gap = body_offset - n - 32;
        if(last < gap / 2)
            body_offset -= last;
    }

    /* Rewriting large objects is expensive -- don't use small gaps.
       This has the additional benefit of block-aligning large bodies. */
    if(length >= 64 * 1024) {
        int min_gap, min_offset;
        if(length >= 512 * 1024)
            min_gap = 4096;
        else if(length >= 256 * 1024)
            min_gap = 2048;
        else
            min_gap = 1024;

        min_offset = ((n + 32 + min_gap - 1) / min_gap + 1) * min_gap;
        body_offset = MAX(body_offset, min_offset);
    }

    return body_offset;
}