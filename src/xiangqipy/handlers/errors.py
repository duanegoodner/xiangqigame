"""
@file errors.py
Handles case where ScriptedPlayer has no moves remaining in list but Game is
not finished.
"""

import sys


def handle_interactive_eof():
    print('\n\nEOF found while running in interactive mode.'
          '\nExiting program')
    sys.exit(0)

