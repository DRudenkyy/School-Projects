# Quick-n-dirty Readme

This archive is based (and butchered) off of:
https://github.com/thewtex/docker-opengl

This archive contains an example Docker container capable of running
Ubuntu 18.04 with a VNC server exposing the internal X server. The
container also hosts a web server, allowing the outside system, the
Docker host, to visit http://localhost:6080 after startup to view
the internal X environment. To start this container, run the following
commands at a shell:

```sh
./build.sh && ./run.sh
```

If you decide the change the startup script or application sources
in the RIT folder, you'll need to run `./build.sh` again. Otherwise,
after one build, you can simply run a fresh instance of the container
by running `./run.sh` by itself.

Closing the original window (XTerm or the application launched by XTerm)
will trigger a shutdown of the container.