FROM debian:latest

WORKDIR /opt/sample-agent
COPY ./build /opt/sample-agent

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
    && mv /opt/sample-agent/sample-agent /usr/bin/ \
    && useradd -m agent \
    && apt remove -y \
    wget \
    gnupg\
    && apt autoremove -y \
    && apt autoclean -y \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /opt/sample-agent

USER agent

CMD ["bash"]
