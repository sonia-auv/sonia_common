#!/bin/bash
set -e

# setup ros environment
source "$ROS_WS_SETUP"

# setup sonia environment if present
if [ -f "$SONIA_WS_SETUP" ];
then
  source "$SONIA_WS_SETUP"
fi

# setup base lib environment if present
if [ -f "$BASE_LIB_WS_SETUP" ];
then
  source "$BASE_LIB_WS_SETUP"
fi

exec "$@"