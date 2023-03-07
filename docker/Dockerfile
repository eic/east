#
# To be used from the main eAST directory, i.e. one level above the "docker" folder
#

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

