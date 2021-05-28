#!/bin/bash
set -e

# setup ros environment
source $ROS_WS_SETUP

exec "$@"