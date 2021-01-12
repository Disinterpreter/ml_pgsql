FROM centos:centos7

ENV PREMAKE_VERSION="5.0.0-alpha15"

##
# Update system
##
RUN \
    yum update -y && \
    yum upgrade -y

##
# Install packages
##
RUN \
    yum install -y \
    gcc \
    gcc-c++ \
    make \
    kernel-devel \
    postgresql-libs \
    postgresql-dev \
    wget \
    curl \
    tar
    
##
# Add premake5 to /usr/local/bin
##
RUN \
    mkdir premake5 && \
    cd premake5 && \
    wget https://github.com/premake/premake-core/releases/download/v${PREMAKE_VERSION}/premake-${PREMAKE_VERSION}-linux.tar.gz && \
    tar -zxvf premake-${PREMAKE_VERSION}-linux.tar.gz && \
    cp premake5 /usr/local/bin && \
    cd .. && \
    rm -rf premake5

##
# Configure workdir
##
WORKDIR /app
COPY . /app

##
# Start building...
##
RUN \
    premake5 gmake2 && \
    cd build && \
    make