int
httpHeaderMatch(AtomPtr header, AtomPtr headers1, AtomPtr headers2)
{
    int rc1, b1, e1, rc2, b2, e2;

    /* Short cut if both sets of headers are identical */
    if(headers1 == headers2)
        return 1;

    rc1 = httpFindHeader(header, headers1->string, headers1->length,
                         &b1, &e1);
    rc2 = httpFindHeader(header, headers2->string, headers2->length,
                         &b2, &e2);

    if(rc1 == 0 && rc2 == 0)
        return 1;

    if(rc1 == 0 || rc2 == 0)
        return 0;

    if(e1 - b1 != e2 - b2)
        return 0;

    if(memcmp(headers1->string + b1, headers2->string + b2, e1 - b1) != 0)
        return 0;

    return 1;
}