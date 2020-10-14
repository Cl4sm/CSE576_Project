import io
import os
from placeholder import CTokenizer

if __name__ == '__main__':
    import argparse

    # parser
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input-file', default='',
                        help='The file to tokenize.')
    args = parser.parse_args()
    assert args.input_file == '' or os.path.isfile(args.input_file)

    # read from standard input, or from input file
    if args.input_file == '':
        source = sys.stdin.read()
    else:
        with io.open(args.input_file, encoding='utf-8') as f:
            source = f.read()

    tokenizer = CTokenizer()
    tokens = tokenizer.tokenize(source)
    print(tokens)

    print("------")

    new_code = tokenizer.detokenize(tokens)
    print(new_code)
