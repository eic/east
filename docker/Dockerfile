FROM electronioncollider/east-base:latest

RUN cmake -Bbuild -S. -DCMAKE_INSTALL_PREFIX=/usr/local \
 && cmake --build build install
