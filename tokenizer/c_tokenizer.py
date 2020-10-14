import re
import os
import string
import random

import clang
from clang.cindex import TokenKind
from sacrebleu.tokenizers import TokenizerV14International

from utils.timeout import TimeoutError, timeout

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

    @timeout(seconds=10)
    def _tokenize(self, code):
        # parse
        tmp_fpath = os.path.join("/tmp", "c_tokenizer-"+"".join(random.choices(string.ascii_letters, k=20))+".c")
        parsed_code = self.idx.parse(tmp_fpath, args=['-std=c11'], unsaved_files=[(tmp_fpath, code)], options=0)

        # return tokens
        tokens = parsed_code.get_tokens(extent=parsed_code.cursor.extent)
        return [(tok.spelling, tok.kind) for tok in tokens]

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
            tokens_n_types = self._tokenize(code)
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
        print(self._tokenize(code))

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
        untok_s = untok_s.replace('CB_COLON', '};').replace(
            'CB_COMA', '},').replace('CB_', '}').replace('OB_', '{')
        return untok_s

if __name__ == "__main__":
    tokenizer = CTokenizer()
    tokens = tokenizer.tokenize(open("tests/test.c").read())
    print("tokens", tokens)
    new_code = tokenizer.detokenize(tokens)
    print(new_code)
