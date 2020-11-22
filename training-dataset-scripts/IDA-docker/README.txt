docker run --privileged -v /tmp/.X11-unix/:/tmp/.X11-unix -e DISPLAY=$DISPLAY --network=host --rm -it ida-docker /bin/bash
