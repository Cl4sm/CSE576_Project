# README

## Tokenizer
Existing tokenizers like BPE(Byte-Pair Encoding), WordPiece are designed for natural languages. In our task, the input and output sequences are C source code. Without knowing the syntax of C code, a tokenizer is likely to parse C code incorrectly and output incorrect C code.

To resolve this problem, we decide to write our own tokenizer based on a compiler library: `libclang`.

Each *\*.c* file is split into several different files, one per each function in the file. #IFDEFs are problems for us since tokenization occurs at a function level and the #IFDEF can't be reasoned about before compilation, but decompiled binaries can either include or exclude that code creating a disparity between the source file and the decompiled file.

To actually tokenize the source and decompiled binaries, we use the tokenizer built into clang. This has worked reasonably well for us.

### Abstraction
however, there have been difficulties with global variable and structures. Clang's tokenizer does not inherrently capture global variables and structures. This is especially true at the function level. Tokenizing at the function level also looses the type context of such variables.

Due to fact that function names and variable names can be arbitrary, there is no vocabulary can contain all function names and variable names. This makes the vocabulary unreasonably large and make the model untrainable.

Compilation does not capture variable names and as such, all decompiled variable names are essentially unusable. We have decided to alleviate this problem by replacing these function and variable names with standardized tokens. The first function that we see is labled `func_1` and similarly for arguments, string literals, and variables it is `arg_1`, `strlit1` and `var_1`. This will allow us to keep all of the potential variable and function names in a small vocabulary and then unmap them to their original names after our model outputs the finished function.
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

### Installation
1. Install `placeholder` project by running `pip install -e .` in `submission/tokenizers`


### Tokenizer
There are two tokenizers in this project. One is `CTokenizer`, the other is `IDATokenizer`.
`CTokenizer` is used to tokenize C source code. To run it, one can do:
```
from placeholder import CTokenizer
tokenizer = CTokenizer()
tokens = tokenizer.tokenize(open("test.c").read())
print(tokens)
```
`tokens` are correctly abstracted C code tokens. They can be detokenized back into C code by using:
```
code = tokenizer.detokenize(tokens)
print(code)
```
Notice that in the example, `test.c` contains one function and it doesn't contain macros like `#include <stdio>`. Our tokenizer assumes there is no macros in the code.

`IDATokenizer` is used to decompile and tokenize a function within a binary.
Due to license issue, we can't ship IDA Pro(a decompiler) to you. But we build a decompilation cache [here](https://doc-0o-7g-docs.googleusercontent.com/docs/securesc/nrqvdihn7o8ifgt32oes1eedm9qfmsbk/o04e1vsdk7rakgplqerjs4svbnvaom5m/1605059925000/14285738738769488385/14285738738769488385/1MftXkP8LEyq56lNAMWkWK_JwfunVoq3k?e=download&authuser=1&nonce=eiovc3lat3ivq&user=14285738738769488385&hash=e0jqpnhpr1i0dshk44j35u20vt8qrg32). You can download it, unzip it and put the resultant folder at `/tmp/ida_tokenizer_cache`. Our `IDATokenizer` will try to read decompilation cache from `/tmp/ida_tokenizer_cache/<cache>`.

If the decompilation cache for a binary exists, the following command will generate valid tokens for a function `<func>` inside binary `bin`
```
from placeholder import IDATokenizer
tokenizer = IDATokenizer()
tokens = tokenizer.tokenize(<bin>, <func>)
print(tokens)
```
And the tokens can be detokenized back into C code by
```
code = tokenizer.detokenize(tokens)
print(code)
```

