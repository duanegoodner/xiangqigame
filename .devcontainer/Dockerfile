FROM ubuntu:22.04

ARG username=duane
ARG user_uid=1000
ARG user_gid=1000
ENV USERNAME ${username}
ENV USER_UID ${user_uid}
ENV USER_GID ${user_gid}

SHELL [ "/bin/bash", "--login", "-c" ]

# Create non-root user (with sudo privileges)
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && apt-get update \
    && apt-get install -y sudo wget \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME \
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*
USER $USERNAME
ENV HOME /home/$USERNAME
ENV WORKSPACE $HOME/workspace

# install some essential packages
# TODO install zip, unzip, build-essential and pkg-config
# TODO install vpkg: https://lindevs.com/install-vcpkg-on-ubuntu
RUN sudo apt-get update --fix-missing && \
    sudo apt-get install -y \
    bzip2 \
    ca-certificates \
    clang-format \
    cmake \
    clang-tidy \
    cppcheck \
    curl \
    gdb \
    g++ \
    git \
    libzip-dev \
    nano \
    make \
    fonts-powerline \
    rsync \
    unzip \
    zsh \
    && sudo apt-get clean && \
    sudo rm -rf /var/lib/apt/lists/* \
    && sudo git clone -q https://github.com/google/googletest.git /googletest \
    && sudo mkdir -p /googletest/build \
    && cd /googletest/build \
    && sudo cmake .. && sudo make && sudo make install \
    && cd / && sudo rm -rf /googletest
    # TODO install https://github.com/pboettch/json-schema-validator
    # For compatibility with pybind11 NEED: cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON ..


# Customize zsh shell
ADD ./.devcontainer/fonts ./fonts
COPY ./.devcontainer/shell_configs/.p10k.zsh ./.devcontainer/shell_configs/.zshrc ./
RUN sh -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" \
    && git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions \
    && git clone https://github.com/zsh-users/zsh-completions ${ZSH_CUSTOM:-${ZSH:-~/.oh-my-zsh}/custom}/plugins/zsh-completions \
    && git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting \
    && git clone https://github.com/zsh-users/zsh-history-substring-search ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-history-substring-search \
    && bind "$terminfo[kcuu1]" history-substring-search-up \
    && bind "$terminfo[kcud1]" history-substring-search-down \
    && git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ~/powerlevel10k \
    && sudo install -m644 ./fonts/MesloLGS_NF_Bold_Italic.ttf /usr/share/fonts/truetype/ \
    && sudo install -m644 ./fonts/MesloLGS_NF_Bold.ttf /usr/share/fonts/truetype/ \
    && sudo install -m644 ./fonts/MesloLGS_NF_Italic.ttf /usr/share/fonts/truetype/ \
    && sudo install -m644 ./fonts/MesloLGS_NF_Regular.ttf /usr/share/fonts/truetype/ \
    && sudo rm -rf ./fonts \
    && sudo chown $USER_UID:$USER_GID ./.p10k.zsh ./.zshrc \
    && sudo mv ./.p10k.zsh ./.zshrc /home/$USERNAME/ \
    && echo exit | script -qec zsh /dev/null >/dev/null

# need this if want all options available when running p10k configure
ENV TERM xterm-256color

WORKDIR $HOME/workspace

# install miniconda and create project env
ENV MINICONDA_VERSION latest
ENV CONDA_DIR $HOME/miniconda3
ENV PATH=$CONDA_DIR/bin:$PATH
COPY ./.devcontainer/initial_conda_env.yml /tmp/ 
RUN wget --quiet https://repo.anaconda.com/miniconda/Miniconda3-$MINICONDA_VERSION-Linux-x86_64.sh -O $HOME/miniconda.sh && \
    chmod +x $HOME/miniconda.sh && \
    # Need to use bash on next line due to issue at: https://github.com/conda/conda/issues/10431
    bash $HOME/miniconda.sh -b -p $CONDA_DIR && \
    rm $HOME/miniconda.sh && \
    echo ". $HOME/miniconda3/etc/profile.d/conda.sh" >> $HOME/.bashrc && \
    echo ". $HOME/miniconda3/etc/profile.d/conda.sh" >> $HOME/.zshrc && \
    conda init bash && \
    conda init zsh && \
    conda install mamba -n base -c conda-forge && \
    sudo chown $USER_UID:$USER_GID /tmp/initial_conda_env.yml && \
    mamba update --name base --channel defaults conda

RUN sudo mkdir $WORKSPACE/env && sudo chown ${USERNAME}:${USERNAME} ${WORKSPACE}/env

RUN chmod +rwx $WORKSPACE/env && \
    mamba env create --prefix $WORKSPACE/env --file /tmp/initial_conda_env.yml && \
    mamba clean --all --yes --force-pkgs-dirs && \
    find ${CONDA_DIR} -follow -type f -name '*.a' -delete && \
    find ${CONDA_DIR} -follow -type f -name '*.pyc' -delete && \
    echo "conda activate ${WORKSPACE}/env" >> ~/.bashrc && \
    echo "conda activate ${WORKSPACE}/env" >> ~/.zshrc

COPY ./.devcontainer/entrypoint.sh /usr/local/bin/
RUN sudo chown $USER_UID:$USER_GID /usr/local/bin/ && \
    sudo chmod u+x /usr/local/bin/entrypoint.sh

COPY ./.devcontainer/xiangqigame_docker.pub ${HOME}/.ssh/authorized_keys
RUN sudo apt-get update

RUN sudo apt-get install openssh-server -y
RUN sudo chown $USER_UID:$USER_GID ${HOME}/.ssh
RUN sudo chown $USER_UID:$USER_GID ${HOME}/.ssh/authorized_keys
RUN sudo chmod 600 ${HOME}/.ssh/authorized_keys
RUN sudo chmod 700 ${HOME}/.ssh

