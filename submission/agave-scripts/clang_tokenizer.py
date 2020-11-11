# Copyright (c) Facebook, Inc. and its affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

import re
from placeholder import CTokenizer
from ctypes.util import find_library
import clang

from dataclasses import dataclass, field
from fairseq.data.encoders import register_tokenizer
from fairseq.dataclass import FairseqDataclass


@dataclass
class ClangTokenizerConfig(FairseqDataclass):
    pass

@register_tokenizer("clangtok", dataclass=ClangTokenizerConfig)
class ClangTokenizer(object):
    def __init__(self, *unused):
        clang.cindex.Config.set_library_file(find_library('clang'))
        self.ctok = CTokenizer()

    def encode(self, x: str) -> str:
        return self.ctok.tokenize(x)

    def decode(self, x: str) -> str:
        return self.ctok.detokenize(x)
