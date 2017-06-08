FROM ubuntu:16.04

RUN apt-get -qq update &&\
        apt-get upgrade && apt-get -qq install -y g++ libboost-all-dev cmake cmake-data

WORKDIR /tmp/recast
COPY src src
COPY headers headers
COPY CMakeLists.txt ./

RUN mkdir build && cd build && cmake .. && make