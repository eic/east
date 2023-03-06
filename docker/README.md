# Docker for eAST

## Install CMake

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