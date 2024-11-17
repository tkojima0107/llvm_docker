#!/bin/sh

# run nvidia default entrypoint
/opt/nvidia/nvidia_entrypoint.sh cat /dev/null

# update ld cache
ldconfig

# run the command passed to the docker run
if [[ $# -gt 0 ]]; then
  exec "$@"
else
  exec /bin/bash --login
fi
