#!/bin/bash
# Build DarkSHINE Simulation Docker image
# Base: RHEL 9.8 (Plow) via UBI 9

set -euo pipefail

IMAGE_NAME="${IMAGE_NAME:-darkshine-simulation}"
IMAGE_TAG="${IMAGE_TAG:-rhel9-full}"
SKIP_ONNX="${SKIP_ONNX:-0}"

echo "=== Building ${IMAGE_NAME}:${IMAGE_TAG} ==="
echo "SKIP_ONNX=${SKIP_ONNX} (set SKIP_ONNX=1 to skip ONNX Runtime)"
echo ""

docker build \
    --build-arg SKIP_ONNX="${SKIP_ONNX}" \
    -t "${IMAGE_NAME}:${IMAGE_TAG}" \
    -f Dockerfile \
    .

echo ""
echo "=== Build complete ==="
echo ""
echo "Run with:"
echo "  docker run --rm -it ${IMAGE_NAME}:${IMAGE_TAG}"
echo ""
echo "To build darkshine-simulation inside the container:"
echo "  docker run --rm -it -v \$(pwd):/src ${IMAGE_NAME}:${IMAGE_TAG} bash -c '"
echo "    source /opt/darkshine/setup.sh"
echo "    cd /src"
echo "    mkdir build install && cd build"
echo "    cmake .. -DCMAKE_INSTALL_PREFIX=../install -DWITH_GEANT4_UIVIS=ON \\"
echo "      -DBUILD_ACTS=ON -DBUILD_HDF5=ON -DBUILD_DANA=ON \\"
echo "      -DBUILD_DSIMU=ON -DBUILD_DDIS=ON -DBUILD_TOOLS=ON -DBUILD_ONNX=ON"
echo "    make -j\$(nproc)"
echo "    make install"
echo "  '"
echo ""
echo "Push to registry:"
echo "  docker tag ${IMAGE_NAME}:${IMAGE_TAG} <registry>/${IMAGE_NAME}:${IMAGE_TAG}"
echo "  docker push <registry>/${IMAGE_NAME}:${IMAGE_TAG}"
