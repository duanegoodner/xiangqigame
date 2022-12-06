from skbuild import setup
from setuptools import find_packages

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
    cmake_install_dir="./src/cpp_modules/game_board_py",
    cmake_source_dir="./src/cpp_modules",
)
