import io
import os
from placeholder import CTokenizer, IDATokenizer

def sanitize_input(args):
    if args.input_file and args.input_bin:
        raise ValueError("You can input tokenize C or binary at a time")

    if not args.input_file and not args.input_bin:
        raise ValueError("At least one of C file or binary file should be provided")

    if args.input_bin and not args.func_name:
        raise ValueError("Function name should be provided for tokenization")

if __name__ == '__main__':
    import argparse

    # parser
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--input-file', default='',
                        help='The C file to tokenize.')
    parser.add_argument('-d', '--build-dir', default=None,
                        help='when building the package, which directory will the compiler be in')
    parser.add_argument('-s', '--build-src-path', default=None,
                        help='when building the package, how is the compiler going to address the source code')
    parser.add_argument('-b', '--input-bin', default='',
                        help='The binary to tokenize.')
    parser.add_argument('-n', '--func-name', default='',
                        help='The function name in the binary to tokenize.')
    args = parser.parse_args()

    sanitize_input(args)

    # choose a proper tokenizer
    if args.input_file:
        with io.open(args.input_file, encoding='utf-8') as f:
            source = f.read()

        tokenizer = CTokenizer(build_dir=args.build_dir, build_src_path=args.build_src_path)
        tokens = tokenizer.tokenize(source)
        print(source)
        print("------")
    elif args.input_bin:
        tokenizer = IDATokenizer()
        tokens = tokenizer.tokenize(args.input_bin, args.func_name)
    else:
        raise

    print(tokens)
    print("------")

    new_code = tokenizer.detokenize(tokens)
    print(new_code)
