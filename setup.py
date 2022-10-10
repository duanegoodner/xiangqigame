from setuptools import setup, Extension

setup(
    ext_modules=[
        Extension(name="xiangqigame.cython_modules.general_moves",
                  sources=["xiangqigame/cython_modules/general_moves.c",
                           "xiangqigame/cython_modules/general_moves.pyx"],
                  include_dirs=["xiangqigame/cython_modules"]),
        Extension(name="xiangqigame.cython_modules.get_all_spaces_occupied_by",
                  sources=[
                      "xiangqigame/cython_modules/get_all_spaces_occupied_by.c",
                      "xiangqigame/cython_modules/get_all_spaces_occupied_by.pyx"],
                  include_dirs=["xiangqigame/cython_modules"]),
        Extension(name="xiangqigame.cython_modules.get_color",
                  sources=[
                      "xiangqigame/cython_modules/get_color.c",
                      "xiangqigame/cython_modules/get_color.pyx"],
                  include_dirs=["xiangqigame/cython_modules"]),
        Extension(name="xiangqigame.cython_modules.get_color",
                  sources=[
                      "xiangqigame/cython_modules/get_color.c",
                      "xiangqigame/cython_modules/get_color.pyx"],
                  include_dirs=["xiangqigame/cython_modules"]),
        Extension(name="xiangqigame.cython_modules.search_spaces",
                  sources=[
                      "xiangqigame/cython_modules/search_spaces.c",
                      "xiangqigame/cython_modules/search_spaces.pyx"],
                  include_dirs=["xiangqigame/cython_modules"]),
    ]
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
