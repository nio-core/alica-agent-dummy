FROM debian:latest AS base
RUN apt update \
    && apt upgrade -y --no-install-recommends \
    && apt install -y --no-install-recommends \
        apt-transport-https \
        ca-certificates \
        wget \
        gnupg \
    && echo "deb https://download.opensuse.org/repositories/network:/messaging:/zeromq:/git-draft/Debian_10/ ./" >> /etc/apt/sources.list \
    && wget https://download.opensuse.org/repositories/network:/messaging:/zeromq:/git-draft/Debian_10/Release.key -O- | apt-key add \
    && apt update \
    && apt install -y --no-install-recommends \
        libzmq3-dev  \
        libcapnp-dev \
    && apt purge -y \
        wget \
        gnupg \
    && apt autoremove -y \
    && apt autoclean -y \
    && rm -rf /var/lib/apt/lists/* \
    && useradd -m agent

FROM base AS build
RUN apt update \
    && apt install -y --no-install-recommends \
        build-essential \
        capnproto \
        pkg-config \
        cmake
WORKDIR /opt/build
COPY . /opt/build
RUN cmake . \
    && make

FROM base
COPY --from=build /opt/build/sample-agent /usr/bin/
USER agent
CMD ["bash"]
