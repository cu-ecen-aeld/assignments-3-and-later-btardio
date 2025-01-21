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
ADD entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
RUN apt-get install -y wget
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu.tar.xz
RUN tar -xvf arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu.tar.xz
RUN apt-get install -y python3
RUN apt-get install -y python3-dev

ENTRYPOINT ["/entrypoint.sh"]




#, "/bin/bash"]

#CMD ["/bin/bash"]

