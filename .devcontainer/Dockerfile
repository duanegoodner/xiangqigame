FROM debian:12

ARG username
ARG user_uid
ARG user_gid
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
    plantuml \
    fonts-powerline \
    rsync \
    unzip \
    zsh 


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


# Set up the environment variables for Miniconda
ENV MINICONDA_VERSION latest
ENV CONDA_DIR $HOME/miniconda3
ENV PATH=$CONDA_DIR/bin:$PATH

# Create directory that will hold our project's conda env
RUN sudo mkdir $WORKSPACE/env && sudo chown ${USERNAME}:${USERNAME} ${WORKSPACE}/env && \
    chmod +rwx $WORKSPACE/env

# Copy env.yml into the container so it is available during env creation
COPY ./.devcontainer/env.yml /tmp/

# Download and install Miniconda, install mamba, and use mamba to create the conda environment
RUN wget --quiet https://repo.anaconda.com/miniconda/Miniconda3-$MINICONDA_VERSION-Linux-x86_64.sh -O $HOME/miniconda.sh && \
    chmod +x $HOME/miniconda.sh && \
    bash $HOME/miniconda.sh -b -p $CONDA_DIR && \
    rm $HOME/miniconda.sh && \
    sudo chown $USER_UID:$USER_GID /tmp/env.yml && \
    /bin/bash -c "source $CONDA_DIR/etc/profile.d/conda.sh && \
    conda init bash && \
    conda init zsh && \
#    conda install mamba -n base -c conda-forge && \
#    mamba update --name base --channel defaults conda && \
    conda env create --prefix $WORKSPACE/env --file /tmp/env.yml && \
    conda clean --all --yes --force-pkgs-dirs" && \
    find ${CONDA_DIR} -follow -type f -name '*.a' -delete && \
    find ${CONDA_DIR} -follow -type f -name '*.pyc' -delete && \
    # Persist the sourcing of conda.sh in .bashrc and .zshrc
    echo "source $CONDA_DIR/etc/profile.d/conda.sh" >> ~/.bashrc && \
    echo "source $CONDA_DIR/etc/profile.d/conda.sh" >> ~/.zshrc && \
    # Ensure that conda is activated in both bash and zsh
    echo "conda activate ${WORKSPACE}/env" >> ~/.bashrc && \
    echo "conda activate ${WORKSPACE}/env" >> ~/.zshrc

# Set the entrypoint script
COPY ./.devcontainer/entrypoint.sh /usr/local/bin/
RUN sudo chown $USER_UID:$USER_GID /usr/local/bin/ && \
    sudo chmod u+x /usr/local/bin/entrypoint.sh

WORKDIR $HOME/workspace/project


# To enable ssh connection to container (e.g. for using PyCharm in container) uncomment
# the remaining lines, generate ssh key pair named xiangqigame_docker, and add save the .pub key
# to ./devcontainer/xiangqigame_docker.pub.

# COPY ./.devcontainer/xiangqigame_docker.pub ${HOME}/.ssh/authorized_keys
# RUN sudo apt-get update

# RUN sudo apt-get install openssh-server -y
# RUN sudo chown $USER_UID:$USER_GID ${HOME}/.ssh
# RUN sudo chown $USER_UID:$USER_GID ${HOME}/.ssh/authorized_keys
# RUN sudo chmod 600 ${HOME}/.ssh/authorized_keys
# RUN sudo chmod 700 ${HOME}/.ssh

