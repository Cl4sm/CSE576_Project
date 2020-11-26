import re
import os
import string
import random
from collections import defaultdict

import clang
from clang.cindex import TokenKind, CursorKind
from sacrebleu.tokenizers import TokenizerV14International

from .utils.timeout import TimeoutError, timeout
from .utils.contexts import cwd_cxt

############### Configuration ###################
CLANG_LIB_PATH = '/usr/local/lib/'
CLANG_INCLUDE_PATH = '/usr/local/lib/clang/11.0.0/include'

TOK_NO_SPACE_BEFORE = {',', ';'}
C_TOKEN2CHAR = {'STOKEN0': "//",
                   'STOKEN1': "/*",
                   'STOKEN2': "*/",
                   'STOKEN3': "/**",
                   'STOKEN4': "**/",
                   'STOKEN5': '"""',
                   'STOKEN6': '\\n'
                   }
C_CHAR2TOKEN = {"//": ' STOKEN0 ',
                   "/*": ' STOKEN1 ',
                   "*/": ' STOKEN2 ',
                   "/**": ' STOKEN3 ',
                   "**/": ' STOKEN4 ',
                   '"""': ' STOKEN5 ',
                   '\\n': ' STOKEN6 '
                   }

# IDA type definition extracted from os.path.join(IDA_PATH, "plugins", "defs.h")
IDA_WHITELIST = ['ll', 'ull', 'll', 'ull', 'll', 'ull', 'uint', 'uchar', 'ushort', 'ulong', 'int8', 'sint8', 'uint8', 'int16', 'sint16',
                 'uint16', 'int32', 'sint32', 'uint32', 'int64', 'sint64', 'uint64', '_BOOL1', '_BOOL2', '_BOOL4', '_BOOL8', 'BYTE',
                 'WORD', 'DWORD', 'LONG', 'BOOL', 'QWORD', '_BYTE', '_WORD', '_DWORD', '_QWORD', '__int64', '__int32', '__int16', '__int8']

############### Initialization ###################
clang.cindex.Config.set_library_path(CLANG_LIB_PATH)


############### The C Tokenizer ###################
class CTokenizer:
    def __init__(self, build_dir=None, build_src_path=None):
        self.idx = clang.cindex.Index.create()
        self.bleu_tokenizer = TokenizerV14International()
        self.replace_dict = {
            'func': {},
            'arg': {},
            'var': {},
            'str_lit': {},
            'global_var': {},
            'attr': {},
            'type': {},
        }
        self.build_dir = build_dir
        self.build_src_path = build_src_path

    @timeout(seconds=10)
    def parse(self, code):
        if self.build_dir:
            with cwd_cxt(self.build_dir):
                # CXTranslationUnit_KeepGoing = 0x200
                # ref: https://clang.llvm.org/doxygen/group__CINDEX__TRANSLATION__UNIT.html
                tmp_fpath = os.path.join(".", "c_tokenizer-"+"".join(random.choices(string.ascii_letters, k=20))+".c")
                args = ['-std=c11', '-I/usr/local/include', '-I%s' % CLANG_INCLUDE_PATH, '-ferror-limit=0']
                tu = self.idx.parse(tmp_fpath, args=args, unsaved_files=[(tmp_fpath, code)], options=0x200)
        else:
            tmp_fpath = os.path.join("/tmp", "c_tokenizer-"+"".join(random.choices(string.ascii_letters, k=20))+".c")
            args = ['-std=c11', '-I%s' % os.path.join(os.path.abspath(IDA_PATH), "plugins"), '-ferror-limit=0']
            tu = self.idx.parse(tmp_fpath, args=args, unsaved_files=[(tmp_fpath, code)], options=0x200)
        return tu

    def _get_abstracted_tokens(self, raw_tokens):
        abstracted_tokens = []
        idx = 0
        while idx < len(raw_tokens):
            token = raw_tokens[idx]
            # retrieve the correct spelling
            if token.spelling == 'struct':
                spelling = token.spelling + " " + raw_tokens[idx+1].spelling
                idx += 2
            else:
                spelling = token.spelling
                idx += 1

            # replace token
            for typ in self.replace_dict.keys():
                if spelling in self.replace_dict[typ]:
                    abstracted_tokens.append(
                        (self.replace_dict[typ][spelling][0], token.kind))
                    break
            else:
                abstracted_tokens.append((token.spelling, token.kind))
        return abstracted_tokens

    def _abstracted_tokenize_v1(self, code):
        # parse
        tu = self.parse(code)

        # get raw_tokens from translation unit
        raw_tokens = list(tu.get_tokens(extent=tu.cursor.extent))
        self.abstract_analysis_v1(tu, code)

        return self._get_abstracted_tokens(raw_tokens)

    def _abstracted_tokenize_v2(self, code):
        # parse
        tu = self.parse(code)

        # get raw_tokens from translation unit
        raw_tokens = list(tu.get_tokens(extent=tu.cursor.extent))
        self.abstract_analysis_v2(tu, code)

        return self._get_abstracted_tokens(raw_tokens)

    def abstracted_tokenize(self, code):
        if self.build_dir:
            tokens = self._abstracted_tokenize_v2(code)
            code = self.detokenize([x[0] for x in tokens])
        return self._abstracted_tokenize_v1(code)

    def _tokenize(self, code):
        # parse
        tu = self.parse(code)

        # get raw_tokens from translation unit
        raw_tokens = tu.get_tokens(extent=tu.cursor.extent)

        # return the tokens directly
        return [(x.spelling, x.kind) for x in raw_tokens]

    def abstract_analysis_v2(self, tu, code):
        def make_name(x): return f"{x}_{len(self.replace_dict[x])//2}"
        def save_replace(typ, c, name=None):
            if name is None:
                name = make_name(typ)
            if c.spelling in self.replace_dict[typ]:
                return
            tup = (name, c.spelling, c.kind)
            self.replace_dict[typ][c.spelling] = tup
            self.replace_dict[typ][tup[0]] = tup

        # add a prefix to make libclang to capture structures and global variables
        with cwd_cxt(self.build_dir), open(self.build_src_path) as f:
            content = f.read()
        new_code = content + '\nstatic int LOL_ABCD_LOL;\n' + code

        # parse new code
        tu = self.parse(new_code)
        #for diag in tu.diagnostics:
        #    print(diag.spelling)

        # process valid tokens, we only process the original code
        found = False
        for c in tu.cursor.walk_preorder():
            tup = None

            # look for original code
            if found == False:
                if c.kind != CursorKind.VAR_DECL or c.spelling != "LOL_ABCD_LOL":
                    continue
                found = True
                continue

            #print(c.kind, c.spelling)
            #if c.kind == CursorKind.INTEGER_LITERAL:
            #    tokens = list(c.get_tokens())
            #    if not tokens:
            #        print(f"macro detected!!!! @ {c.location}")


            if c.kind == CursorKind.FUNCTION_DECL:
                save_replace('func', c)

            elif c.kind == CursorKind.CALL_EXPR:
                save_replace('func', c)

            elif c.kind == CursorKind.PARM_DECL:
                save_replace('arg', c)

            elif c.kind == CursorKind.VAR_DECL:
                # in the new code, a VAR_DECL cursor might be introduced by us for global variables
                if c.spelling in self.replace_dict['global_var']:
                    continue
                save_replace('var', c)

            elif c.kind == CursorKind.STRING_LITERAL:
                new_str = '"'+make_name('str_lit').replace('_', '').strip()+'"'
                save_replace('var', c, name=new_str)

            elif c.kind == CursorKind.MEMBER_REF_EXPR:
                save_replace('attr', c)

            elif c.kind == CursorKind.TYPE_REF:
                save_replace('type', c)

        # now process the ast again to find undefined variable use and record them as global variables
        found = False
        for c in tu.cursor.walk_preorder():
            tup = None

            # look for original code
            if found == False:
                if c.kind != CursorKind.VAR_DECL or c.spelling != "LOL_ABCD_LOL":
                    continue
                found = True
                continue

            if c.kind == CursorKind.DECL_REF_EXPR:
                if c.spelling in self.replace_dict['global_var']:
                    continue
                if all(c.spelling not in self.replace_dict[x] for x in ['func', 'arg', 'var', 'str_lit', 'attr', 'type']):
                    tup = (make_name('global_var'), c.spelling, c.kind)
                    self.replace_dict['global_var'][c.spelling] = tup
                    self.replace_dict['global_var'][tup[0]] = tup

    def abstract_analysis_v1(self, tu, code):
        def make_name(x): return f"{x}_{len(self.replace_dict[x])//2}"
        def save_replace(typ, c, name=None):
            if name is None:
                name = make_name(typ)
            if c.spelling in self.replace_dict[typ]:
                return
            tup = (name, c.spelling, c.kind)
            self.replace_dict[typ][c.spelling] = tup
            self.replace_dict[typ][tup[0]] = tup

        if "__noreturn" in code:
            code = code.replace("__noreturn", "")
            tu = self.parse(code)

        global_vars = set()
        # process global variables which are not valid out of the program contexts
        for diag in tu.diagnostics:
            # we don't care about warnings
            if diag.severity < clang.cindex.Diagnostic.Error:
                continue
            #print(diag.spelling)
            # usage of unknown global variable is "undeclared identifier" error
            res = re.search(f"use of undeclared identifier '(.*)'", diag.spelling)
            if not res:
                continue
            var_name = res.group(1)
            if var_name in IDA_WHITELIST:
                continue
            global_vars.add(var_name)
            # now add this abastraction into our mapping
            if var_name not in self.replace_dict['global_var']:
                tup = (make_name('global_var'), var_name, "global_var")
                self.replace_dict['global_var'][var_name] = tup
                self.replace_dict['global_var'][tup[0]] = tup

        declarations = "#include <math.h>\n#include <defs.h>\nstatic int LOL_ABCD_LOL;\n"
        for var in global_vars:
            declarations += f"extern {var};\n"
        new_code = declarations + code

        # parse new code
        tu = self.parse(new_code)
        #for diag in tu.diagnostics:
        #    print(diag.spelling)

        # process valid tokens
        found = False
        for c in tu.cursor.walk_preorder():
            tup = None

            # look for original code
            if found == False:
                if c.kind != CursorKind.VAR_DECL or c.spelling != "LOL_ABCD_LOL":
                    continue
                found = True
                continue


            if c.kind == CursorKind.FUNCTION_DECL:
                save_replace('func', c)

            elif c.kind == CursorKind.CALL_EXPR:
                save_replace('func', c)

            elif c.kind == CursorKind.PARM_DECL:
                save_replace('arg', c)

            elif c.kind == CursorKind.VAR_DECL:
                # in the new code, a VAR_DECL cursor might be introduced by us for global variables
                if c.spelling in self.replace_dict['global_var']:
                    continue
                save_replace('var', c)

            elif c.kind == CursorKind.STRING_LITERAL:
                new_str = '"'+make_name('str_lit').replace('_', '').strip()+'"'
                save_replace('str_lit', c, new_str)


    def process_string(self, tok, char2tok, tok2char, is_comment):
        # a token processing function ported from TransCoder
        if is_comment:
            tok = re.sub(r' +', ' ', tok)
            tok = re.sub(r"(.)\1\1\1\1+", r"\1\1\1\1\1", tok)
            if len(re.sub(r'\W', '', tok)) < 2:
                return ''
        tok = tok.replace(' ', ' ▁ ')
        for char, special_token in char2tok.items():
            tok = tok.replace(char, special_token)
        if tok.startswith(' STOKEN0'):
            if tok.endswith('\n'):
                tok = tok[:-1]
            tok += ' ENDCOM'
        tok = tok.replace('\n', ' STRNEWLINE ')
        tok = tok.replace('\t', ' TABSYMBOL ')
        tok = re.sub(r' +', ' ', tok)
        tok = self.bleu_tokenizer(tok)
        tok = re.sub(r' +', ' ', tok)
        for special_token, char in tok2char.items():
            tok = tok.replace(special_token, char)
        tok = tok.replace('\r', '')

        return tok

    def indent_lines(self, lines):
        # a function handles indentation ported from TransCoder
        prefix = ''
        for i, line in enumerate(lines):
            line = line.strip()
            if re.match(r'CB_COLON|CB_COMA|CB_', line):
                prefix = prefix[2:]
                line = prefix + line
            elif line.endswith('OB_'):
                line = prefix + line
                prefix += '  '
            else:
                line = prefix + line
            lines[i] = line
        untok_s = '\n'.join(lines)
        return untok_s
        
    def tokenize(self, code):
        assert isinstance(code, str)
        tokens = []

        # pre-process: strip soft return
        code = code.replace("\r", "")

        # use libclang to parse the code
        tokens_n_types = self.abstracted_tokenize(code)
        try:
            tokens_n_types = self.abstracted_tokenize(code)
        except TimeoutError:
            print("Timeout Error")
            return []
        except Exception as e:
            print(e)
            print(f"Unknown error during parsing {code}")

        # post-process: strip comments and tokenize literals
        for tok, typ in tokens_n_types:

            # comments are stripped
            if typ == TokenKind.COMMENT:
                continue

            # use sacrebleu to process literals
            if typ == TokenKind.LITERAL:
                tokens.append(self.process_string(tok, C_CHAR2TOKEN, C_TOKEN2CHAR, False))
                continue

            tokens.append(tok)

        return tokens

    def detokenize(self, tokens):
        assert isinstance(tokens, list)

        # stringify the tokens and then make it looks like a valid C code
        code = ' '.join(tokens)
        code = code.replace('ENDCOM', '\n').replace('▁', ' SPACETOKEN ')

        # parse the pseudo C code with libclang
        try:
            tokens_n_types = self._tokenize(code)
        except TimeoutError:
            print("Timeout Error")
            return ""
        except Exception as e:
            print("Fail to detokenize by libclang")
            print(code)
            print(e)
            return ""

        # translate the tokens to new tokens by translating
        # special symbols back to normal characters
        i = 0
        new_tokens = []
        while i < len(tokens_n_types):
            tok, typ  = tokens_n_types[i]

            # ignore comment token
            if typ == TokenKind.COMMENT:
                i += 1
                continue
            # detokenize literals
            elif typ ==  TokenKind.LITERAL:
                new_tok = tok.replace('STRNEWLINE', '\n').replace('TABSYMBOL', '\t')\
                             .replace(' ', '').replace('SPACETOKEN', ' ')
                new_tokens.append(new_tok)
            # closing curly brace is tricky, usually there is no space between two tokens
            # if the first token is closing curly brace
            elif tok == '}':
                if i < len(tokens_n_types)-1 and tokens_n_types[i+1][0] == ';':
                    new_tokens += ['CB_COLON', 'NEW_LINE']
                    i += 2
                    continue
                if i < len(tokens_n_types)-1 and tokens_n_types[i+1][0] == ',':
                    new_tokens += ['CB_COMA', 'NEW_LINE']
                    i += 2
                    continue
                new_tokens += ['CB_', 'NEW_LINE']
            # standardize output code format
            elif tok == '{':
                new_tokens += ['OB_', 'NEW_LINE']
            elif tok == '*/':
                new_tokens += ['*/', 'NEW_LINE']
            elif tok == ';':
                new_tokens += [';', 'NEW_LINE']
            else:
                new_tokens.append(tok)

            # handle tokens that usually is used without space in between
            if i < len(tokens_n_types)-1 and tokens_n_types[i+1][0] in TOK_NO_SPACE_BEFORE:
                next_token = tokens_n_types[i+1][0]
                new_tokens[len(new_tokens)-1] += next_token
                if next_token == ';':
                    new_tokens.append('NEW_LINE')
                i += 2
                continue
            i += 1

        lines = re.split(r'NEW_LINE', ' '.join(new_tokens))
        untok_s = self.indent_lines(lines)
        untok_s = untok_s.replace('CB_COLON', '};').replace('CB_COMA', '},')\
                         .replace('CB_', '}').replace('OB_', '{')
        return untok_s

from .ida_tokenizer import IDA_PATH
