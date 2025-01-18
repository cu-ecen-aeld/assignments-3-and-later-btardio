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
ENTRYPOINT ["/entrypoint.sh"]




#, "/bin/bash"]

#CMD ["/bin/bash"]

