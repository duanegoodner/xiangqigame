import signal
import sys


def sig_int_exit(sig, frame):
    print('\n SIG INT received. Exiting program.')
    sys.exit(0)


def sig_eof_exit(sig, frame):
    print('EOF received. Exiting.')
    sys.exit(0)


def set_signal_handlers():
    signal.signal(signal.SIGINT, sig_int_exit)