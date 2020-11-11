import idc
import idaapi
import ida_hexrays
import idautils
import ida_funcs
import traceback
import os

log_file_path = os.path.dirname(os.path.abspath(__file__))
log_file_path = os.path.join(
    log_file_path, '..', '..', 'ida_tokenizer.log')
log_file = open(log_file_path, 'w+')


class IDATokenizer:

    def __init__(self, file_path):
        self.file_path = file_path
        log_file.write("File Path: %s\n" % (file_path))
        idc.auto_wait()
        self.cfuncs = self.decompile()
        try:
            visitor = my_super_visitor()
            log_file.write("Visitor: %s" % (type(visitor)))
            visitor.apply_to(self.cfuncs[0].body, None)
        except Exception as e:
            traceback.print_exc(file=log_file)
        #for func in self.cfuncs:
        #    self.visit_func(func)

    def decompile(self):
        ida_hexrays.init_hexrays_plugin()
        cfuncs = []
        for function in idautils.Functions():
            try:
                cfunc = ida_hexrays.decompile(function)
                log_file.write("CFUNC: %s\n" % (cfunc))
                cfuncs.append(cfunc)
            except Exception as e:
                log_file.write("ERROR: %s - %s\n" %
                               (e, idc.get_func_name(function)))
                #traceback.print_exc(file=log_file)
        return cfuncs

    def visit_func(self, cfunc):
        v = my_super_visitor()
        #v.apply_to(cfunc.body, None)
        #log_file.write("Visitor: %s\n" % (v))


class my_super_visitor(ida_hexrays.ctree_visitor_t):

    def __init__(self):
        ida_hexrays.ctree_visitor_t.__init__(self, ida_hexrays.CV_FAST)

    def visit_insn(self, i):
        log_file.write("VISIT INSN: %s\n" % (i))
        log_file.write("    CBLOCK: %s\n" % (i.cblock))
        log_file.write("    CEXPR: %s\n" % (i.cexpr))
        log_file.write("    CIF: %s\n" % (i.cfor))
        return 0

    def visit_expr(self, e):
        if e.op != ida_hexrays.cot_asg:
            return 0
        return 0


if __name__ == '__main__':
    curr_loc = os.path.dirname(os.path.abspath(__file__))
    exec_file = 'test_1'
    tokenizer = IDATokenizer(os.path.join(
        curr_loc, 'tests', 'exec_files', exec_file))

log_file.close()
idc.qexit(0)
