#
#
#

FROM buddhasystem/python:3.10.1-bullseye-cmake

ARG HEPMC3_VERSION=3.2.5

WORKDIR /tmp

RUN wget https://hepmc.web.cern.ch/hepmc/releases/HepMC3-${HEPMC3_VERSION}.tar.gz && tar xfvz HepMC3-3.2.5.tar.gz
RUN mkdir hepmc3-build && \
cd hepmc3-build &&  \
cmake -DCMAKE_INSTALL_PREFIX=/opt/hepmc3     \
        -DHEPMC3_ENABLE_ROOTIO:BOOL=OFF            \
        -DHEPMC3_ENABLE_PROTOBUFIO:BOOL=OFF        \
        -DHEPMC3_ENABLE_TEST:BOOL=OFF              \
        -DHEPMC3_INSTALL_INTERFACES:BOOL=ON        \
        -DHEPMC3_BUILD_STATIC_LIBS:BOOL=OFF        \
        -DHEPMC3_BUILD_DOCS:BOOL=OFF     \
        -DHEPMC3_ENABLE_PYTHON:BOOL=ON   \
        -DHEPMC3_PYTHON_VERSIONS=2.7     \
        -DHEPMC3_Python_SITEARCH27=../hepmc3-install/lib/python2.7/site-packages \
        ../HepMC3-3.2.5 && \
        make -j4 && \
        make install

RUN rm -fr /tmp

WORKDIR /user
