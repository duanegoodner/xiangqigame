"""
@file signals.py

Handlers for signals received when Game is running in terminal UI.
"""

import signal
import sys


catchable_signals = set(signal.Signals) - {signal.SIGKILL, signal.SIGSTOP}
signals_to_ignore = catchable_signals - {signal.SIGINT}


def sig_int_exit(sig, frame):
    print('\n\nSIGINT received. Exiting program.')
    sys.exit(0)


def set_ignore_handlers():
    for ignored_signal in signals_to_ignore:
        signal.signal(ignored_signal, signal.SIG_IGN)


def set_signal_handlers():
    signal.signal(signal.SIGINT, sig_int_exit)
    set_ignore_handlers()

