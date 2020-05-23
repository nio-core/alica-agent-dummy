FROM debian:latest

WORKDIR /opt/sample-agent
COPY ./ /opt/sample-agent

RUN apt update \
    && apt install -y \
    wget \
    gnupg \
    && echo "deb https://download.opensuse.org/repositories/network:/messaging:/zeromq:/git-draft/Debian_10/ ./" >> /etc/apt/sources.list \
    && wget https://download.opensuse.org/repositories/network:/messaging:/zeromq:/git-draft/Debian_10/Release.key -O- | apt-key add \
    && apt update \
    && apt install -y \
    libzmq3-dev  \
    libcapnp-dev \
    make \
    cmake \
    g++ \
    gcc \
    pkg-config \
    python3-pip \
    && pip3 install conan \
    && useradd -m agent \
    && conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan \
    && cmake . \
    && make -j4 \
    && make install \
    && ldconfig \
    && pip3 uninstall -y conan \
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
    && apt autoclean -y \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /opt/sample-agent

USER agent

CMD ["bash"]
