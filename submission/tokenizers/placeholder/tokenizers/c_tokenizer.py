import re
import os
import string
import random
from collections import defaultdict

import clang
from clang.cindex import TokenKind, CursorKind
from sacrebleu.tokenizers import TokenizerV14International

from .utils.timeout import TimeoutError, timeout

############### Configuration ###################
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

############### Initialization ###################
clang.cindex.Config.set_library_path('/usr/local/lib/')


############### The C Tokenizer ###################
class CTokenizer:
    def __init__(self):
        self.idx = clang.cindex.Index.create()
        self.bleu_tokenizer = TokenizerV14International()
        self.replace_dict = {
            'func': {},
            'arg': {},
            'var': {},
            'str_lit': {},
            'global_var': {},
        }

    @timeout(seconds=10)
    def parse(self, code):
        tmp_fpath = os.path.join("/tmp", "c_tokenizer-"+"".join(random.choices(string.ascii_letters, k=20))+".c")

        # CXTranslationUnit_KeepGoing = 0x200
        # ref: https://clang.llvm.org/doxygen/group__CINDEX__TRANSLATION__UNIT.html
        tu = self.idx.parse(tmp_fpath, args=['-std=c11'], unsaved_files=[(tmp_fpath, code)], options=0x200)

        #print(code)
        #for c in tu.cursor.walk_preorder():
        #    print(str(c.kind)+'\t'+str(c.spelling))
        return tu

    def _tokenize(self, code, replace_tokens=False):
        # parse
        tu = self.parse(code)

        # get raw_tokens from translation unit
        raw_tokens = tu.get_tokens(extent=tu.cursor.extent)

        if not replace_tokens:
             return [(token.spelling, token.kind) for token in raw_tokens]

        # record token replacement by abstraction analysis
        self.token_abstractor(tu, code)

        # now perform token replacement
        abstracted_tokens = []
        for token in raw_tokens:
            for typ in self.replace_dict.keys():
                if token.spelling in self.replace_dict[typ]:
                    abstracted_tokens.append(
                        (self.replace_dict[typ][token.spelling][0], token.kind))
                    break
            else:
                abstracted_tokens.append((token.spelling, token.kind))
        return abstracted_tokens

    def token_abstractor(self, tu, code):
        def make_name(x): return f"{x}_{len(self.replace_dict[x])//2}"

        global_vars = set()
        # process global variables which are not valid out of the program contexts
        for diag in tu.diagnostics:
            # we don't care about warnings
            if diag.severity < clang.cindex.Diagnostic.Error:
                continue
            # usage of unknown global variable is "undeclared identifier" error
            res = re.search(f"use of undeclared identifier '(.*)'", diag.spelling)
            if not res:
                continue
            var_name = res.group(1)
            global_vars.add(var_name)
            # now add this abastraction into our mapping
            if var_name not in self.replace_dict['global_var']:
                tup = (make_name('global_var'), var_name, "global_var")
                self.replace_dict['global_var'][var_name] = tup
                self.replace_dict['global_var'][tup[0]] = tup

        declarations = ""
        for var in global_vars:
            declarations += f"extern {var};\n"
        new_code = declarations + code

        # parse new code
        tu = self.parse(new_code)

        # process valid tokens
        for c in tu.cursor.walk_preorder():
            tup = None
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


    def abstract_name_replace(self, token):
        return next(x[0] for x in self.replace_dict['func'].keys() + self.replace_dict['var'].keys())

        ##MISSING GLOBAL DECLARATIONS
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
        try:
            tokens_n_types = self._tokenize(code, replace_tokens=True)
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

if __name__ == "__main__":
    tokenizer = CTokenizer()
    tokens = tokenizer.tokenize(open("./tests/c_files/test_1.c").read())
    print("tokens", tokens)
    new_code = tokenizer.detokenize(tokens)
    print(new_code)