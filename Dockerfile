FROM ubuntu:22.04
RUN apt-get update
RUN apt-get install -y build-essential ruby cmake
ADD docker_install.sh /docker_install.sh
ADD runner_install.sh /runner_install.sh
RUN sh /docker_install.sh
RUN sh /runner_install.sh
ENV RUNNER_ALLOW_RUNASROOT=1
RUN /actions-runner/config.sh remove --url https://github.com/cu-ecen-aeld/assignment-1-btardio --token AFAF6AORHSIAUUEWISU3VGDHRMP2C
RUN apt-get install -y vim
RUN apt-get install -y wget
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu.tar.xz
RUN tar -xvf arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu.tar.xz
RUN apt-get install -y python3
# RUN apt-get install -y python3-dev
RUN apt-get install -y gdb
RUN apt-get install -y file
RUN apt-get install -y syslog-ng
ADD entrypoint.sh /entrypoint.sh
RUN echo "set number" >> /root/.vimrc
RUN echo "set laststatus=2" >> /root/.vimrc
RUN echo "filetype plugin indent on" >> /root/.vimrc
RUN echo "set tabstop=4" >> /root/.vimrc
RUN echo "set shiftwidth=4" >> /root/.vimrc
RUN echo "set expandtab" >> /root/.vimrc
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]




#, "/bin/bash"]

#CMD ["/bin/bash"]

