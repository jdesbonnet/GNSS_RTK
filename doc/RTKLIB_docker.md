## Building RTKLIB demo5 using docker

```
FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update
RUN apt install -y qtcreator qtbase5-dev qt5-qmake cmake
RUN apt install -y git g++

RUN mkdir /opt/rtklib
WORKDIR /opt/rtklib
RUN git clone https://github.com/rtklibexplorer/RTKLIB/
WORKDIR /opt/rtklib/RTKLIB
RUN cd app/qtapp; qmake; make -j

```
