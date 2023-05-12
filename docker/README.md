# Docker for eAST

## Dockerfiles

For convenience, we factor the `eAST` image building into three steps:
* An image containing most prerequisites for Geant4 and eAST, which are installable via `apt`
* An image based on the previous one, containing the Geant4 installation
* eAST -- built on top of the Geant4 image

The eAST image needs to be built from the eAST folder, and point to the Dockerfile contained
in the `docker` folder under it. Example:

```bash
docker build . -t buddhasystem/east:0.1 -f docker/Dockerfile
```

The tag name corresponds to the temporary location of this image
in a Gocker Hub registry, it's not meant for production.

## Optional: install CMake from source

In the current version we use `apt-get` to install CMake. If that is for some
reason problematic (e.g. the list of repoistories cannot be updated, or some are
unavailable) CMake can be installed from source as described below.

```docker
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz && \
tar xvzf cmake-${CMAKE_VERSION}.tar.gz && \
cd cmake-${CMAKE_VERSION} && \
./bootstrap && \
make -j 4 && \
make install && \
cmake --version
```