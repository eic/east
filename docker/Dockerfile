#
# This Dockerfile is used to buils an image for the eAST application.
#
# It should be used from the main eAST directory, i.e. one level above the "docker" folder,
# so the "-f" option of Docker build is required to point to this file.

FROM buddhasystem/east-geant4:0.1


# Switch to the nominal user directory, clean up
WORKDIR /user
COPY . /user/east

RUN cd east && rm -fr .git* docker build && \
mkdir build && \
export MAKEFLAGS=-j4 && \
cmake -DCMAKE_INSTALL_PREFIX=/opt/east -DHepMC3=~/opt/hepmc3 -S . -B build && \
cmake --build build && \
cmake --install build

RUN rm -fr /user/east/build
ENV PATH=/opt/east/bin:$PATH
