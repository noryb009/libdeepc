FROM ubuntu

# Upgrade core packages
RUN apt-get update && apt-get upgrade -y 

# Install dependecies of libdeepc
RUN apt-get install -y gcc-8 python3 make
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 50
RUN update-alternatives --install /usr/bin/python python /usr/bin/python3 50

# Install nice to haves
RUN apt-get install -y vim tmux git strace gdb valgrind

WORKDIR /libdeepc
