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

WORKDIR /tmp
RUN wget https://github.com/gcc-mirror/gcc/archive/refs/tags/releases/gcc-13.1.0.tar.gz \
	&& tar xvf gcc-13.1.0.tar.gz \
	&& cd gcc-releases-gcc-13.1.0 \
	&& ./contrib/download_prerequisites \
	&& mkdir build \
	&& cd build \
	&& ../configure --prefix=/opt/gcc-13.1.0/ --enable-languages=c,c++ --disable-multilib \
	&& make -j`nproc` \
	&& make install \
	&& rm -rf /tmp/gcc-*

ENV LD_LIBRARY_PATH /opt/gcc-13.1.0/lib:/opt/gcc-13.1.0/lib64:$LD_LIBRARY_PATH

RUN git clone -b llvmorg-16.0.6 https://github.com/llvm/llvm-project.git \
		&& mkdir /tmp/llvm-project/build \
		&& cd /tmp/llvm-project/build \
		&& cmake -G "Ninja"  -DCMAKE_BUILD_TYPE="Release" \
                   -DCMAKE_C_COMPILER=/opt/gcc-13.1.0/bin/gcc \
                   -DCMAKE_CXX_COMPILER=/opt/gcc-13.1.0/bin/g++ \
                   -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;compiler-rt;libclc;lld;lldb;openmp;polly;pstl;mlir;flang;" \
				   -DLLVM_ENABLE_RUNTIMES="libcxx;libunwind;libcxxabi" \
                   -DCMAKE_INSTALL_PREFIX=/opt/llvm16.0.6 \
                   -DLLVM_ENABLE_ASSERTIONS=On \
                   ../llvm \
		&& cmake --build . \
		&& cmake --install . \
		&& cd / \
		&& rm -rf /tmp/llvm-project

ENV LD_LIBRARY_PATH=
RUN echo "/opt/llvm16.0.6/lib" >> /etc/ld.so.conf.d/20-llvm.conf
RUN { \
        echo "/opt/gcc-13.1.0/lib"; \
        echo "/opt/gcc-13.1.0/lib64"; \
        } >> /etc/ld.so.conf.d/10-gcc-12.conf
RUN ldconfig
ENV PATH /opt/llvm16.0.6/bin:${PATH}
WORKDIR /root

CMD ["/bin/bash", "--login"]
