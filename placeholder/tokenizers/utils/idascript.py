import time
import json

import ida_pro
import ida_auto
import ida_funcs
import ida_hexrays
import ida_segment
import idautils

BEGIN_BANNER = "===================================IDA SCRIPT START============================================="
END_BANNER = "===================================IDA SCRIPT END============================================="

"""
name
start_ea
end_ea
code
"""

def get_funcs():
    start = time.time()
    gen = idautils.Functions()
    funcs_ea = list(gen)
    funcs = [ida_funcs.get_func(ea) for ea in funcs_ea]
    return funcs

def in_range(segm, ea):
    return segm.start_ea <= ea < segm.end_ea

def main():
    """
    for each function in .text segment, we extract
    name, start_ea, end_ea, code, size attributes from IDA
    """
    results = {}

    start = time.time()
    funcs = get_funcs()

    text_segm = ida_segment.get_segm_by_name(".text")
    for func in funcs:
        # we only care about functions in .text segment
        if not in_range(text_segm, func.start_ea):
            continue

        name = ida_funcs.get_func_name(func.start_ea)
        code = ida_hexrays.decompile_func(func, None)
        results[name] = {   "name": name,
                            "start_ea": func.start_ea,
                            "end_ea": func.end_ea,
                            "code": code.__str__(),
                            "size": func.size()
                            }
    print("JSON:")
    print(json.dumps({"time": time.time() - start, "data": results}))


# wait for the analysis to complete
ida_auto.auto_wait()

# actually do stuff
print(BEGIN_BANNER)
main()
print(END_BANNER)

# quit to stop GUI from poping up
ida_pro.qexit(0)
