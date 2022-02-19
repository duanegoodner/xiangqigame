import argparse
import xiangqigame.app as app


def main():

    parser = argparse.ArgumentParser()
    parser.add_argument('--use_cur_env', required=False, action="store_true")
    args = parser.parse_args()
    app.run()


if __name__ == '__main__':
    main()
