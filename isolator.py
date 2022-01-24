from subprocess import call
import venv
import os
import pkgutil


def build_venv():
    isolated_env_name = 'xiangqi_venv'
    env_builder = venv.EnvBuilder(with_pip=True)
    env_builder.create(isolated_env_name)

    python_bin = isolated_env_name + '/bin/python'
    pip_bin = isolated_env_name + '/bin/pip'

    # requirements = pkgutil.get_data('xiangqi_game', '../requirements.txt').decode().split()
    # print(requirements)
    install_cmd = [pip_bin, 'install', '-r', 'requirements.txt']
    call(install_cmd)

    # call([pip_bin, 'install', '.'])
    call([python_bin, '-m', 'xiangqi_game'])

    # call([python_bin, '-m', 'run'])


if __name__ == '__main__':
    build_venv()
