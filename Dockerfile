# Base image
FROM ubuntu:18.04

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    gdb \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /usr/src/app

# Copy the source code to the container
COPY . .

