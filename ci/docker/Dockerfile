ARG BASEIMG
FROM $BASEIMG

ARG NUM_PROCS

# build the software of this repository
COPY . /sourcecode
RUN mkdir /build \
  && cd /build \
  && cmake -DCMAKE_INSTALL_PREFIX=/opt/hello ../sourcecode \
  && make -j$NUM_PROCS \
  && make install
