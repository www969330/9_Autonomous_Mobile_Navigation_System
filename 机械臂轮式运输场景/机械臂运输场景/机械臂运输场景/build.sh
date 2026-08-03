#!/bin/bash
set -e

cd "$(dirname "$0")"

echo "Building ROS Melodic Tiago Docker image..."
sg docker -c "docker-compose build"
echo "Build completed!"
