FROM debian:latest

RUN apt update \
    && apt install -y wget gnupg

RUN echo "deb https://download.opensuse.org/repositories/network:/messaging:/zeromq:/git-draft/Debian_10/ ./" >> /etc/apt/sources.list \
    && wget https://download.opensuse.org/repositories/network:/messaging:/zeromq:/git-draft/Debian_10/Release.key -O- | apt-key add

RUN apt update \
    && apt install -y \
    libzmq3-dev  \
    libcapnp-dev \
    make \
    cmake \
    g++ \
    gcc \
    pkg-config \
    python3-pip \
    && pip3 install conan

RUN useradd -m agent \
    && mkdir -p /opt/sample-agent

WORKDIR /opt/sample-agent
COPY ./ /opt/sample-agent

# Build
RUN conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan \
    && cmake . \
    && make -j4 \
    && make install \
    && ldconfig \
    && rm -rf /opt/sample-agent

# Cleanup
RUN pip3 uninstall -y conan \
    && apt remove -y \
    make \
    cmake \
    g++ \
    gcc \
    pkg-config \
    wget \
    gnupg\
    python3-pip \
    && apt autoremove -y \
    && rm -rf /var/lib/apt/lists/*

USER agent

CMD ["sample-agent"]
