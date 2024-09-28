#FROM debian:wheezy
FROM ubuntu:20.04

ENV HOME /root

RUN apt-get update && apt-get install -y locales && rm -rf /var/lib/apt/lists/* \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8
ENV LANG en_US.utf8
ENV LC_ALL C.UTF-8

# update and install dependencies
RUN         apt-get update \
                && apt-get install -y \
                    software-properties-common \
                    wget \
                && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
                && apt-get update \
                && apt-get install -y \
                    make \
                    git \
                    curl \
                && apt-get install -y cmake \
                && apt-get install -y \
                    gcc-7 g++-7 gcc-7-base \
                && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 100 \
                && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 100


RUN apt-get update && apt-get install -y \
    git \
    libxml2-dev \
    python \
    build-essential \
    make \
    gcc \
    python-dev \
    locales \
    zsh \
    wget \
    gdb

RUN echo $('gcc --version')


# # Install Zsh
#RUN git clone git://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh
#RUN cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc && \
#    chsh -s /bin/zsh

# Install SystemC
RUN mkdir -p "/home/uvm-fi/Downloads" && \
    cd "/home/uvm-fi/Downloads/" && \
    wget "https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz" && \
    tar xfv systemc-2.3.3.tar.gz && \
    cd systemc-2.3.3 && \
    mkdir -p "/usr/local/systemc-2.3.3" && \
    ./configure --prefix=/usr/local/systemc-2.3.3/ --enable-debug --enable-pthreads && \
    make -j 8 && \
    make -j 8 install && \
    cd ../ && \
    rm -r systemc-2.3.3

ENV LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:/usr/local/systemc-2.3.3/lib-linux64"
ENV PKG_CONFIG_PATH "${PKG_CONFIG_PATH}:/usr/local/systemc-2.3.3/lib-linux64/pkgconfig"

RUN mkdir -p "/home/uvm-fi/Downloads" && \
    cd "/home/uvm-fi/Downloads/" && \
    wget "https://www.accellera.org/images/downloads/drafts-review/uvm-systemc-10-beta3tar.gz" && \
    tar xfv uvm-systemc-10-beta3tar.gz && \
    cd uvm-systemc-1.0-beta3 && \
    mkdir -p "/usr/local/uvm-systemc" && \
    ./configure --with-systemc=/usr/local/systemc-2.3.3 --prefix=/usr/local/uvm-systemc && \
    make -j 8 && \
    make -j 8 install && \
    cd ../ && \
    rm -r uvm-systemc-1.0-beta3 

# export LD_LIBRARY_PATH="/usr/local/uvm-systemc/lib-linux64:$LD_LIBRARY_PATH"
ENV LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:/usr/local/uvm-systemc/lib-linux64"
ENV PKG_CONFIG_PATH "${PKG_CONFIG_PATH}:/usr/local/uvm-systemc/lib-linux64/pkgconfig"

RUN apt-get clean all

CMD ["/bin/zsh"]
