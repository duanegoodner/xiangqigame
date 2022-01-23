import os
from subprocess import call


def display_object(obj: object, spec_type=None):
    print(obj)


def clear_screen():
    _ = call('clear' if os.name == 'posix' else 'cls')
