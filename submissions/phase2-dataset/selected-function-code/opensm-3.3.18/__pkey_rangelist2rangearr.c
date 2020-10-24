 ***************************************************/

static void __pkey_rangelist2rangearr(
    cl_list_t    * p_list,
    uint64_t  ** * p_arr,
    unsigned     * p_arr_len)
{
    uint64_t   tmp_pkey;
    uint64_t * p_pkeys;
    cl_list_iterator_t list_iterator;

    list_iterator= cl_list_head(p_list);
    while( list_iterator != cl_list_end(p_list) )
    {
       p_pkeys = (uint64_t *)cl_list_obj(list_iterator);
       p_pkeys[0] &= 0x7fff;
       p_pkeys[1] &= 0x7fff;
       if (p_pkeys[0] > p_pkeys[1])
       {
           tmp_pkey = p_pkeys[1];
           p_pkeys[1] = p_pkeys[0];
           p_pkeys[0] = tmp_pkey;
       }
       list_iterator = cl_list_next(list_iterator);
    }
