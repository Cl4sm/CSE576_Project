# README

|overview|

## Tokenizer
Each *\*.c* file is split into several different files, one per each function in the file. #IFDEFs are problems for us since tokenization occurs at a function level and the #IFDEF can't be reasoned about before compilation, but decompiled binaries can either include or exclude that code creating a disparity between the source file and the decompiled file. To actually tokenize the source and decompiled binaries, we use the tokenizer built into clang. This has worked reasonably well for us, however, there have been difficulties with global variable and structures. Clang's tokenizer does not inherrently capture global variables and structures. This is especially true at the function level. Tokenizing at the function level also looses the type context of such variables.

### Abstraction
Unless for some reason a binary is compiled with excessive debugging information (which would never be the case in some deployed software), there will be a severe out-of-vocabulary issue between the variable and function names of the decompiled code and the original source code. Compilation does not capture variable names and as such, all decompiled variable names are essentially unusable. We have decided to alleviate this problem by replacing these function and variable names with standardized tokens. The first function that we see is labled `func_1` and similarly for arguments, string literals, and variables it is `arg_1`, `strlit1` and `var_1`. This will allow us to keep all of the potential variable and function names in a small vocabulary and then unmap them to their original names after our model outputs the finished function.
```python
            if c.kind == CursorKind.FUNCTION_DECL:
                tup = (make_name('func'), c.spelling, c.kind)
                self.replace_dict['func'][c.spelling] = tup
                self.replace_dict['func'][tup[0]] = tup

            elif c.kind == CursorKind.CALL_EXPR:
                if not c.spelling in self.replace_dict['func']:
                    tup = (make_name('func'), c.spelling, c.kind)
                    self.replace_dict['func'][c.spelling] = tup
                    self.replace_dict['func'][tup[0]] = tup

            elif c.kind == CursorKind.PARM_DECL:
                tup = (make_name('arg'), c.spelling, c.kind)
                self.replace_dict['arg'][c.spelling] = tup
                self.replace_dict['arg'][tup[0]] = tup

            elif c.kind == CursorKind.VAR_DECL:
                # in the new code, a VAR_DECL cursor might be introduced by us for global variables
                if c.spelling in self.replace_dict['global_var']:
                    continue
                tup = (make_name('var'), c.spelling, c.kind)
                self.replace_dict['var'][c.spelling] = tup
                self.replace_dict['var'][tup[0]] = tup

            elif c.kind == CursorKind.STRING_LITERAL:
                if not c.spelling in self.replace_dict['str_lit']:
                    new_str = '"'+make_name('str_lit').replace('_', '').strip()+'"'
                    tup = (new_str, c.spelling, c.kind)
                    self.replace_dict['str_lit'][c.spelling] = tup
                    self.replace_dict['str_lit'][tup[0]] = tup
```
## Model

## Current status

## How to run the code
