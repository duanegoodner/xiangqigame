import argparse
import sys

p = argparse.ArgumentParser()
p.add_argument('--argument', required=False, default="default main arg")
p.add_argument('-a', required='--argument' in sys.argv, default="default a arg") #only required if --argument is given
p.add_argument('-b', required='--argument' in sys.argv, default="default b arg") #only required if --argument is given


def main(*args):
    command_args = p.parse_args(*args)
    print(command_args)


if __name__ == "__main__":
    main()