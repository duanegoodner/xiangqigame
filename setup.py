from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext

extension_modules = [
    Pybind11Extension(
        name="GameBoardPy",
        sources=[
            "src/cpp_modules/game_board_py/game_board_py.cpp",
            "src/cpp_modules/game_board/game_board.cpp",
            "src/cpp_modules/move_calculator/board_utilities.cpp",
            "src/cpp_modules/move_calculator/move_calculator.cpp"
            ],
        include_dirs=["src/cpp_modules/game_board",
                      "src/cpp_modules/move_calculator"],
        language="c++",
        cxx_std=17,
        extra_compile_args=["-Wall", "-Wextra", "-O3"]
        )
]

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
    ext_modules=extension_modules
)
