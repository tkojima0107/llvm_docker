FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get -y update
RUN apt-get -y install gcc g++
RUN apt-get -y install git
RUN apt-get -y install cmake
RUN apt-get -y install python3
RUN apt-get -y install ninja-build
RUN apt-get -y install python3-distutils
RUN apt-get -y install libelf-dev libffi-dev
RUN apt-get -y install bzip2 flex wget
RUN apt-get -y install libnuma-dev gnupg2
RUN wget -q -O - https://repo.radeon.com/rocm/rocm.gpg.key | apt-key add -
COPY rocm.list /etc/apt/sources.list.d/rocm.list
RUN apt-get -y update
RUN apt-get -y install rocm-hip-libraries

WORKDIR /tmp
RUN wget https://github.com/gcc-mirror/gcc/archive/refs/tags/releases/gcc-11.3.0.tar.gz \
	&& tar xvf gcc-11.3.0.tar.gz \
	&& cd gcc-releases-gcc-11.3.0 \
	&& ./contrib/download_prerequisites \
	&& mkdir build \
	&& cd build \
	&& ../configure --prefix=/opt/gcc-11.3.0/ --enable-languages=c,c++ --disable-multilib \
	&& make -j`nproc` \
	&& make install \
	&& rm -rf /tmp/gcc-*

ENV LD_LIBRARY_PATH /opt/gcc-11.3.0/lib:/opt/gcc-11.3.0/lib64:/opt/rocm/lib:$LD_LIBRARY_PATH

RUN git clone -b llvmorg-13.0.1 https://github.com/llvm/llvm-project.git \
		&& mkdir /tmp/llvm-project/build \
		&& cd /tmp/llvm-project/build \
		&& cmake -G "Ninja"  -DCMAKE_BUILD_TYPE="Release" \
                   -DCMAKE_C_COMPILER=/opt/gcc-11.3.0/bin/gcc \
                   -DCMAKE_CXX_COMPILER=/opt/gcc-11.3.0/bin/g++ \
                   -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;compiler-rt;libclc;libcxx;libcxxabi;libunwind;lld;lldb;openmp;parallel-libs;polly;pstl;mlir;flang;" \
                   -DCMAKE_INSTALL_PREFIX=/opt/llvm13.0.1 \
                   -DLLVM_ENABLE_ASSERTIONS=On \
                   ../llvm \
		&& cmake --build . \
		&& cmake --install . \
        && mkdir -p /tmp/llvm-project/omp-build/install  \
        && cd /tmp/llvm-project/omp-build \
        && LD_LIBRARY_PATH=/usr/local/cuda/compat:${LD_LIBRARY_PATH} \
           PATH=/opt/llvm13.0.1/bin:${PATH} \
           CC=clang CXX=clang++ cmake -Wno-dev -G Ninja -DCMAKE_BUILD_TYPE=Release \
		   -DLIBOMPTARGET_AMDGCN_GFXLIST="gfx700;gfx701;gfx801;gfx803;gfx805;gfx810;gfx900;gfx902;gfx904;gfx906;gfx908;gfx909;gfx90c;gfx1010;gfx1011\
;gfx1012;gfx1013;gfx1030;gfx1031;gfx1032;gfx1033;gfx1034;gfx1035" \
           -DLLVM_ENABLE_PROJECTS="clang;openmp" ../llvm \
           -DCMAKE_INSTALL_PREFIX=./install \
        && cmake --build . \
        && cmake --install . \
        && find /tmp/llvm-project/omp-build/install -name libomptarget* -exec cp {} /opt/llvm13.0.1/lib/ \; \
		&& rm -rf /tmp/llvm-project

RUN echo 'export LD_LIBRARY_PATH=/opt/llvm13.0.1/lib:/opt/rocm/lib/:${LD_LIBRARY_PATH}' >> /etc/profile
RUN echo 'export PATH=/opt/llvm13.0.1/bin:${PATH}' >> /etc/profile
WORKDIR /root

CMD ["/bin/bash", "--login"]
