How to launch the docker container for each IDA instance:
	docker run --privileged -v /tmp/.X11-unix/:/tmp/.X11-unix -e DISPLAY=$DISPLAY --network=host --rm -it ida-docker /bin/bash

How to run master on the host:
	python master.py

How to launch decompilation slave instances inside the containers:
	cd code && python slave.py -l log.txt -H 172.17.0.1 -p 8080
