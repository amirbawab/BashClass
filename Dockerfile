FROM ubuntu:16.04
ENV HOME /root
RUN apt-get update
RUN apt-get install -y git curl wget
RUN apt-get install -y g++ make
RUN curl "https://cmake.org/files/v3.9/cmake-3.9.0-rc3.tar.gz" > /tmp/cmake.tar.gz
RUN cd /tmp; tar -xf cmake.tar.gz
WORKDIR "/tmp/cmake-3.9.0-rc3"
RUN ./configure; make; make install
RUN curl -L "https://downloads.sourceforge.net/project/boost/boost/1.64.0/boost_1_64_0.tar.gz" > /tmp/boost.tar.gz
RUN cd /tmp; tar -xf boost.tar.gz
WORKDIR "/tmp/boost_1_63_0/"
RUN ./bootstrap.sh --prefix=/usr/local; ./b2 install
RUN cd $HOME; git   clone https://github.com/amirbawab/BashClass
WORKDIR $HOME/BashClass
RUN git submodule update --init --recursive
RUN cmake .; make
CMD /bin/bash
