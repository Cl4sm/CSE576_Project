# README

In the past 2 weeks, we implemented tokenizers for tokenizing C code and decompiled code into valid tokens.
Based on the tokens, we trained a `fairseq` model and obtained some preliminary results.

## Tokenizer
Existing tokenizers like BPE(Byte-Pair Encoding), WordPiece are designed for natural languages. In our task, the input and output sequences are C source code. Without knowing the syntax of C code, a tokenizer is likely to parse C code incorrectly and output incorrect C code.

To resolve this problem, we decide to write our own tokenizers. Due to the nature of our project, we need two tokenizers: one for tokenizing C code and the other for tokenizing binary code. We build our C code tokenizer based on a compiler library `libclang` and our binary tokenizer based on `libclang` and a decompiler `IDA Pro`. We call them `CTokenizer` and `IDATokenizer` respectively.

`CTokenizer` is able to tokenize a C function into tokens. For example, it is able to tokenize the following function:
```
int main(int argc, char **argv)
{
    printf("Welcome to the fun house, enter your name: ");
    char name[32];
    scanf("%30s", name);
    printf("%d\n", global_var);

    for (int i = 0; i < 10; i++)
    {
        printf("Counting: %d\n", i);
    }
    printf("Done!\n");
    printf("Goodbye %s\n", name);
}
```
into a sequence of tokens: `['int', 'main', '(', 'int', 'argc', ',', 'char', '*', '*', 'argv', ')', '{', 'printf', '(', '" Welcome ▁ to ▁ the ▁ fun ▁ house , ▁ enter ▁ your ▁ name : ▁ "', ')', ';', 'char', 'name', '[', '32', ']', ';', 'scanf', '(', '" % 30s "', ',', 'name', ')', ';', 'printf', '(', '" % d \\n "', ',', 'global_var', ')', ';', 'for', '(', 'int', 'i', '=', '0', ';', 'i', '<', '10', ';', 'i', '++', ')', '{', 'printf', '(', '" Counting : ▁ % d \\n "', ',', 'i', ')', ';', '}', 'printf', '(', '" Done ! \\n "', ')', ';', 'printf', '(', '" Goodbye ▁ % s \\n "', ',', 'name', ')', ';', '}']`

## Abstraction
Due to fact that function names and variable names can be arbitrary, our vocabulary is unreasonably large after tokenizing the whole dataset. Since they are not relevant to the goal of project, we decided to standardize names of functions and variables.
By this method, we are able to drastically reduce the vocabulary size and direct our model to focus on learning control flow related information.

After the abstraction, a function
```
int main(int argc, char **argv)
{
    printf("Welcome to the fun house, enter your name: ");
    char name[32];
    scanf("%30s", name);
    printf("%d\n", global_var);

    for (int i = 0; i < 10; i++)
    {
        printf("Counting: %d\n", i);
    }
    printf("Done!\n");
    printf("Goodbye %s\n", name);
}
```
will be tokenized into tokens `['int', 'func_0', '(', 'int', 'arg_0', ',', 'char', '*', '*', 'arg_1', ')', '{', 'func_1', '(', '" strlit0"', ')', ';', 'char', 'var_0', '[', '32', ']', ';', 'func_2', '(', '" strlit1"', ',', 'var_0', ')', ';', 'func_1', '(', '" strlit2"', ',', 'global_var_0', ')', ';', 'for', '(', 'int', 'var_1', '=', '0', ';', 'var_1', '<', '10', ';', 'var_1', '++', ')', '{', 'func_1', '(', '" strlit3"', ',', 'var_1', ')', ';', '}', 'func_1', '(', '" strlit4"', ')', ';', 'func_1', '(', '" strlit5"', ',', 'var_0', ')', ';', '}']`.

However, there have been difficulties with global variables and structures. `libclang` does not inherently capture global variables and structures. This is especially true at the function level. Tokenizing at the function level also loses the type context of such variables.
To resolve this issue, we perform our analysis twice, once to capture information loss and other for token substitution. However, our abstraction is still not complete: it can't standardize a struct attribute if the struct is unknown. But it is able to substitute known tokens in the second run so our abstraction is still sound.

## Model

## Preliminary Results
We include some of our preliminary results in `submission/examples`. Comment of each example is also included in the example files.

## How to run the code

### Installation
1. Install `placeholder` project by running `pip install -e .` in `submission/tokenizers`


### Tokenizer
There are two tokenizers in this project. One is `CTokenizer`, and the other is `IDATokenizer`.
`CTokenizer` is used to tokenize C source code. It can be invoked as
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
Notice that in the example, `test.c` contains one function and devoid of preprocessor directives such as header includes. Our tokenizer assumes there are no such directives in the code. Examples can be found at `submission/tokenizers/placeholder/tokenizers/tests/c_files/test_1.c`

`IDATokenizer` is used to decompile and tokenize a function within a binary.
Due to license issue, we can't ship `IDA Pro`(the decompiler we use for decompilation) to you. Instead, we share the decompiler output [here](https://doc-0o-7g-docs.googleusercontent.com/docs/securesc/nrqvdihn7o8ifgt32oes1eedm9qfmsbk/o04e1vsdk7rakgplqerjs4svbnvaom5m/1605059925000/14285738738769488385/14285738738769488385/1MftXkP8LEyq56lNAMWkWK_JwfunVoq3k?e=download&authuser=1&nonce=eiovc3lat3ivq&user=14285738738769488385&hash=e0jqpnhpr1i0dshk44j35u20vt8qrg32). You can download it, unzip it and put the resultant folder at `/tmp/ida_tokenizer_cache`. Our `IDATokenizer` will try to read decompilation cache from `/tmp/ida_tokenizer_cache/<cache>`.

If the decompilation output for a binary exists, the following command will generate valid tokens for a function `<func>` inside binary `<bin>`
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

### Generate training dataset
To generate training dataset, one can run `python gen_training_dataset.py -d <dataset_folder> -o <output_folder>` in `submission/training-dataset-scripts`.
It will generate a folder containing both source code(`raw_code`) and decompiled code(`raw_decompiled_code`) and another version of them with comment stripped away(folder with a prefix `stripped`).
