from skbuild import setup
from setuptools import find_packages
# from setuptools import setup, Extension
from Cython.Build import cythonize


import sys

# Available at setup time due to pyproject.toml
# from pybind11 import get_cmake_dir
# from pybind11.setup_helpers import Pybind11Extension, build_ext



setup(
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    include_package_data=True,
    package_data={"": ["*.json"]},
    entry_points={
        "console_scripts": [
            "play_xiangqi = xiangqigame.__main__:main"
        ]
    },
    zip_safe=False,
    ext_modules=cythonize("./src/xiangqigame/cython_modules/*.pyx"),
    cmake_install_dir="./src/cpp_modules/game_board_py",
    cmake_source_dir="./src/cpp_modules",
    
    
)
