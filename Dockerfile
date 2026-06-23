# Dockerfile for DarkSHINE Simulation (darkshine-simulation)
# Base: Red Hat Enterprise Linux 9.8 (Plow) — UBI 9
# Target: all -DBUILD_*=ON

FROM registry.access.redhat.com/ubi9/ubi:latest

LABEL maintainer="SII-inpac"
LABEL description="DarkSHINE Simulation — full build on RHEL 9.8"
LABEL version="1.0"

# ── System setup ──────────────────────────────────────────────
RUN dnf -y update && \
    dnf -y install dnf-plugins-core && \
    dnf -y install https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm && \
    dnf config-manager --set-enabled crb && \
    dnf -y install \
        # Build toolchain
        cmake \
        gcc-c++ \
        gcc \
        make \
        git \
        wget \
        tar \
        gzip \
        which \
        # ROOT + Geant4 system deps
        mesa-libGL-devel \
        mesa-libGLU-devel \
        libX11-devel \
        libXpm-devel \
        libXft-devel \
        libXext-devel \
        libXt-devel \
        libGLEW \
        glew-devel \
        ftgl-devel \
        # Core dependencies
        gsl-devel \
        yaml-cpp-devel \
        xerces-c-devel \
        nlohmann-json-devel \
        eigen3-devel \
        # HDF5
        hdf5-devel \
        # ACTS deps
        boost-devel \
        # ONNX Runtime deps
        protobuf-devel \
        protobuf-compiler \
        # Common
        openssl-devel \
        expat-devel \
        zlib-devel \
        pcre2-devel \
    && dnf clean all

# ── Environment ────────────────────────────────────────────────
ENV INSTALL_PREFIX=/opt/darkshine
ENV PATH=${INSTALL_PREFIX}/bin:${PATH}
ENV LD_LIBRARY_PATH=${INSTALL_PREFIX}/lib:${INSTALL_PREFIX}/lib64:${LD_LIBRARY_PATH}
ENV CMAKE_PREFIX_PATH=${INSTALL_PREFIX}

WORKDIR /tmp/build

# ── Geant4 10.06.p03 ──────────────────────────────────────────
# EPEL 9 does not package Geant4 10.06 — build from source
ARG GEANT4_VERSION=10.06p03
# Geant4 10.06 + GCC 11: suppress maybe-uninitialized warnings promoted to errors
RUN wget -q https://github.com/Geant4/geant4/archive/v${GEANT4_VERSION}.tar.gz -O geant4.tar.gz && \
    tar xzf geant4.tar.gz && \
    cd geant4-${GEANT4_VERSION} && \
    mkdir build && cd build && \
    cmake .. \
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
        -DCMAKE_CXX_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=stringop-overflow" \
        -DGEANT4_INSTALL_DATA=ON \
        -DGEANT4_USE_OPENGL_X11=ON \
        -DGEANT4_USE_RAYTRACER_X11=ON \
        -DGEANT4_BUILD_MULTITHREADED=ON \
    && make -j$(nproc) \
    && make install \
    && cd /tmp/build && rm -rf geant4-${GEANT4_VERSION} geant4.tar.gz

# Source Geant4 environment
ENV GEANT4_DIR=${INSTALL_PREFIX}
RUN echo "source ${INSTALL_PREFIX}/bin/geant4.sh" >> /etc/profile.d/darkshine.sh

# ── ROOT 6.30 ──────────────────────────────────────────────────
# Build from source for consistent version control
ARG ROOT_VERSION=6-30-06
RUN wget -q https://github.com/root-project/root/archive/refs/tags/v${ROOT_VERSION}.tar.gz -O root.tar.gz && \
    tar xzf root.tar.gz && \
    cd root-${ROOT_VERSION} && \
    mkdir build && cd build && \
    cmake .. \
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
        -DCMAKE_CXX_STANDARD=17 \
        -Droot7=OFF \
        -Dtmva=ON \
        -Deve=ON \
        -Dx11=ON \
        -Dopengl=ON \
        -Dfail-on-missing=ON \
    && make -j$(nproc) \
    && make install \
    && cd /tmp/build && rm -rf root-${ROOT_VERSION} root.tar.gz

# Source ROOT environment
RUN echo "source ${INSTALL_PREFIX}/bin/thisroot.sh" >> /etc/profile.d/darkshine.sh

# ── ACTS (ykrsama fork, xuliang-v30 branch) ────────────────────
ARG ACTS_BRANCH=xuliang-v30
RUN git clone -b ${ACTS_BRANCH} --depth 1 \
        https://github.com/ykrsama/acts.git acts-src && \
    cd acts-src && \
    mkdir build && cd build && \
    cmake .. \
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
        -DCMAKE_CXX_STANDARD=17 \
        -DACTS_BUILD_PLUGIN_JSON=ON \
    && make -j$(nproc) \
    && make install \
    && cd /tmp/build && rm -rf acts-src

# ── ONNX Runtime ───────────────────────────────────────────────
# NOTE: ONNX build takes ~30 min; skip with --build-arg SKIP_ONNX=1
ARG ONNX_VERSION=1.19.2
ARG SKIP_ONNX=0
RUN if [ "${SKIP_ONNX}" = "0" ]; then \
      wget -q https://github.com/microsoft/onnxruntime/archive/refs/tags/v${ONNX_VERSION}.tar.gz -O onnx.tar.gz && \
      tar xzf onnx.tar.gz && \
      cd onnxruntime-${ONNX_VERSION} && \
      ./build.sh \
          --config Release \
          --build_shared_lib \
          --parallel $(nproc) \
          --skip_tests \
          --use_cuda=OFF \
      && cp -r include/onnxruntime ${INSTALL_PREFIX}/include/ && \
      find build/Linux/Release -name 'libonnxruntime*.so*' -exec cp -a {} ${INSTALL_PREFIX}/lib/ \; && \
      cd /tmp/build && rm -rf onnxruntime-${ONNX_VERSION} onnx.tar.gz; \
    else \
      echo "Skipping ONNX Runtime build (SKIP_ONNX=1)"; \
    fi

# ── DarkSHINE setup script ─────────────────────────────────────
RUN printf '#!/bin/bash\n\
export DSS_DIR=%s\n\
export PATH=${DSS_DIR}/bin:${PATH}\n\
export LD_LIBRARY_PATH=${DSS_DIR}/lib:${DSS_DIR}/lib64:${LD_LIBRARY_PATH}\n\
source ${DSS_DIR}/bin/geant4.sh\n\
source ${DSS_DIR}/bin/thisroot.sh\n\
echo "DarkSHINE environment loaded (DSS_DIR=${DSS_DIR})"\n' \
    ${INSTALL_PREFIX} > ${INSTALL_PREFIX}/setup.sh && \
    chmod +x ${INSTALL_PREFIX}/setup.sh

# ── Verification ────────────────────────────────────────────────
RUN echo "=== Installed versions ===" && \
    gcc --version | head -1 && \
    cmake --version | head -1 && \
    echo "Geant4: $(cat ${INSTALL_PREFIX}/share/Geant4-*/Geant4PackageCache.cmake 2>/dev/null | grep geant4-source | head -1 || echo 'installed')" && \
    ${INSTALL_PREFIX}/bin/root --version 2>/dev/null | head -1 && \
    echo "GSL: $(gsl-config --version 2>/dev/null)" && \
    echo "yaml-cpp: $(pkg-config --modversion yaml-cpp 2>/dev/null || echo 'installed')" && \
    echo "XercesC: $(pkg-config --modversion xerces-c 2>/dev/null || echo 'installed')" && \
    echo "nlohmann_json: $(pkg-config --modversion nlohmann_json 2>/dev/null || echo 'installed')" && \
    echo "Eigen3: $(pkg-config --modversion eigen3 2>/dev/null || echo 'installed')" && \
    echo "HDF5: $(h5cc -showconfig 2>/dev/null | grep 'HDF5 Version' || echo 'installed')" && \
    echo "Boost: $(grep '#define BOOST_VERSION' /usr/include/boost/version.hpp 2>/dev/null | awk '{print $3}' || echo 'installed')" && \
    ls ${INSTALL_PREFIX}/lib/libonnxruntime* 2>/dev/null && echo "ONNX Runtime: $(ls ${INSTALL_PREFIX}/lib/libonnxruntime.so* 2>/dev/null | head -1)" || true && \
    echo "=== Verification complete ==="

# ── Cleanup ────────────────────────────────────────────────────
RUN rm -rf /tmp/build
WORKDIR /work

# Default: source the environment
CMD ["/bin/bash", "-c", "source /opt/darkshine/setup.sh && exec /bin/bash"]
