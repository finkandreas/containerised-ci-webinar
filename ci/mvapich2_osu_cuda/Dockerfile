FROM  nvidia/cuda:11.2.0-devel-ubuntu20.04 as builder

LABEL "rfm.features"="osu-micro-benchmarks;mpi;serial;openmp;mvapich2"
LABEL "rfm.cc"="mpicc"
LABEL "rfm.cxx"="mpic++"
LABEL "rfm.ftn"="mpif90"

ARG TARGET=haswell
ARG SPACK_TAG=v0.20.1

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    git \
    gfortran \
    curl \
    python3 \
    libpmi2-0-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone -b ${SPACK_TAG} --depth 1 https://github.com/spack/spack.git

RUN mkdir -p /opt/spack/spack-env/ \
&&  (echo "spack:" \
&&   echo "  packages: " \
&&   echo "    all:"\
&&   echo "      target: [${TARGET}]"\
&&   echo "    cuda: " \
&&   echo "      externals: " \
&&   echo "      - spec: cuda@11.2.152 " \
&&   echo "        prefix: /usr/local/cuda " \
&&   echo "      buildable: False " \
&&   echo "  specs:" \
&&   echo "  - osu-micro-benchmarks@5.9 +cuda ^mvapich2@2 +cuda process_managers=slurm" \
&&   echo "  concretizer:" \
&&   echo "    unify: true" \
&&   echo "  config:" \
&&   echo "    install_tree:"\
&&   echo "      root: /opt/spack/software" \
&&   echo "  view: /opt/spack/view") > /opt/spack/spack-env/spack.yaml

RUN . /spack/share/spack/setup-env.sh && \
    spack compiler find && \
    cd /opt/spack/spack-env && \
    spack env activate . && \
    spack install --fail-fast && \
    spack gc -y

RUN . /spack/share/spack/setup-env.sh && \
    cd /opt/spack/spack-env && \
    spack env activate --sh -d . >> /etc/profile.d/z10_spack_environment.sh  && \
    spack env activate . && \
    # Add the mvapich2 lib path to ld.so.conf.d directory
    spack find --format='{prefix.lib}' mvapich2 > /etc/ld.so.conf.d/mvapich2.conf

FROM nvidia/cuda:11.2.0-runtime-ubuntu20.04

LABEL "rfm.features"="osu-micro-benchmarks;mpi;serial;openmp;cuda"
LABEL "rfm.cc"="mpicc"
LABEL "rfm.cxx"="mpic++"
LABEL "rfm.ftn"="mpif90"

RUN apt-get update && apt-get install -y  \
    libpmi2-0-dev \
    libatomic1 \
    libgfortran5 \
    libgomp1
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /opt/spack /opt/spack
COPY --from=builder /etc/profile.d/z10_spack_environment.sh /etc/profile.d/z10_spack_environment.sh

# For the sarus mpi hook
COPY --from=builder /etc/ld.so.conf.d/mvapich2.conf /etc/ld.so.conf.d/mvapich2.conf
RUN ldconfig

ENTRYPOINT ["/bin/bash", "--rcfile", "/etc/profile", "-l", "-c"]
