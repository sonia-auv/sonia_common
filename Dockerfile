ARG BASE_IMAGE="ros:melodic-robot"

FROM ${BASE_IMAGE}

ARG BUILD_DATE
ARG VERSION
ARG SONIA_USER=sonia
ARG SONIA_UID=50000
ARG BASE_LIB_NAME=sonia_msgs

LABEL maintainer="club.sonia@etsmtl.net"
LABEL net.etsmtl.sonia-auv.base_lib.build-date=${BUILD_DATE}
LABEL net.etsmtl.sonia-auv.base_lib.version=${VERSION}
LABEL net.etsmtl.sonia-auv.base_lib.name=${BASE_LIB_NAME}

## COMMON ENV
ENV ROS_LANG_DISABLE=genlisp:geneus
ENV SONIA_USER=${SONIA_USER}
ENV SONIA_HOME=/home/${SONIA_USER}
ENV SONIA_UID=${SONIA_UID}
ENV ROS_WS_SETUP=/opt/ros/${ROS_DISTRO}/setup.bash

## ADD EXTRA DEPENDENCIES (GIT and ROS Remote Debuging)
RUN apt-get update \
    && apt-get install -y libyaml-cpp-dev \
    openssh-client \
    gdb

## ENV FOR BASE LIB
ENV BASE_LIB_WS=${SONIA_HOME}/base_lib_ws
ENV BASE_LIB_WS_SETUP=${BASE_LIB_WS}/devel/setup.bash
ENV BASE_LIB_NAME=${MODULE_LIB_NAME}
ENV BASE_LIB_PATH=${BASE_LIB_WS}/src/${BASE_LIB_NAME}

RUN useradd --uid ${SONIA_UID} --create-home ${SONIA_USER}

## Adding support for vscode extension volume caching
RUN mkdir -p ${SONIA_HOME}/.vscode-server/extensions \
    && chown -R ${SONIA_USER}: ${SONIA_HOME}/.vscode-server

WORKDIR ${BASE_LIB_WS}

COPY . ${BASE_LIB_PATH}
RUN bash -c "source ${ROS_WS_SETUP}; catkin_make"

RUN chown -R ${SONIA_USER}: ${BASE_LIB_WS}
