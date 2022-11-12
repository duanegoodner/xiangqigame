
from setuptools import setup, Extension
from Cython.Build import cythonize

setup(
    ext_modules=cythonize("src/xiangqigame/cython_modules/*.pyx")
    # ext_modules=[
    #     Extension(name="xiangqigame.cython_modules.cython_board_utilities",
    #               sources=[
    #                   "xiangqigame/cython_modules/cython_board_utilities.c",
    #                   "xiangqigame/cython_modules/cython_board_utilities.pyx"]),
    #     Extension(name="xiangqigame.cython_modules.general_moves",
    #               sources=["xiangqigame/cython_modules/general_moves.c",
    #                        "xiangqigame/cython_modules/general_moves.pyx"])
    # ]
)

# setup(
#     name='xiangqigame',
#     version='0.0.1',
#     description='A board game',
#     url='#',
#     author='duanegoodner',
#     install_requires=['numpy'],
#     author_email='dmgoodner@gmail.com',
#     packages=setuptools.find_packages(),
#     package_data={"": ["*.json"]},
#     include_package_data=True,
#     zip_safe=False
# )
