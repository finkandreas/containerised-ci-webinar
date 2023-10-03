FROM ubuntu:22.04 as builder

ARG TARGET=haswell
ARG SPACK_TAG=v0.20.1

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    git \
    gfortran \
    python3 \
    libpmi2-0-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone -b ${SPACK_TAG} --depth 1 https://github.com/spack/spack.git

RUN mkdir -p /opt/spack/spack-env/ \
&& export MPI_SPEC=$([ "${TARGET}" = 'haswell' ] && echo 'mpich@3.4.3 pmi=pmi2' || echo 'mvapich2@2 process_managers=slurm') \
&& (echo "spack: " \
&&  echo "  packages: "\
&&  echo "    all:"\
&&  echo "      target: [${TARGET}]"\
&&  echo "  specs:" \
&&  echo "  - osu-micro-benchmarks@6 ^${MPI_SPEC}" \
&&  echo "  concretizer:" \
&&  echo "    unify: true" \
&&  echo "  config:" \
&&  echo "    install_tree:"\
&&  echo "      root: /opt/spack/software" \
&&  echo "  view: /opt/spack/view") > /opt/spack/spack-env/spack.yaml

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
    # Add the mpi lib path to ld.so.conf.d directory
    export MPI_PROVIDER=$([ "${TARGET}" = 'haswell' ] && echo 'mpich' || echo 'mvapich2') && \
    spack find --format='{prefix.lib}' ${MPI_PROVIDER} > /etc/ld.so.conf.d/mpi.conf

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y  \
    libpmi2-0-dev \
    libatomic1 \
    libgfortran5 \
    libgomp1 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /opt/spack /opt/spack
COPY --from=builder /etc/profile.d/z10_spack_environment.sh /etc/profile.d/z10_spack_environment.sh

# For the sarus mpi hook
COPY --from=builder /etc/ld.so.conf.d/mpi.conf /etc/ld.so.conf.d/mpi.conf
RUN ldconfig

ENTRYPOINT ["/bin/bash", "--rcfile", "/etc/profile", "-l", "-c"]
