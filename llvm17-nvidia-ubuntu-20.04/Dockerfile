FROM nvidia/cuda:11.8.0-devel-ubuntu20.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get -y update
RUN apt-get -y install gcc g++
RUN apt-get -y install git
RUN apt-get -y install make texinfo
RUN apt-get -y install python3
RUN apt-get -y install ninja-build
RUN apt-get -y install python3-distutils
RUN apt-get -y install libelf-dev libffi-dev
RUN apt-get -y install bzip2 flex wget
RUN apt-get -y install libc6-dev-i386

WORKDIR /tmp

RUN wget https://cmake.org/files/v3.30/cmake-3.30.5-linux-x86_64.sh \
	&& mkdir /opt/cmake \
	&& sh cmake-3.30.5-linux-x86_64.sh --prefix=/opt/cmake --skip-license \
	&& rm cmake-3.30.5-linux-x86_64.sh

ENV PATH /opt/cmake/bin:${PATH}

RUN wget https://github.com/gcc-mirror/gcc/archive/refs/tags/releases/gcc-13.2.0.tar.gz \
	&& tar xvf gcc-13.2.0.tar.gz \
	&& cd gcc-releases-gcc-13.2.0 \
	&& ./contrib/download_prerequisites \
	&& mkdir build \
	&& cd build \
	&& ../configure --prefix=/opt/gcc-13.2.0/ --enable-languages=c,c++ --disable-multilib \
	&& make -j`nproc` \
	&& make install \
	&& rm -rf /tmp/gcc-*

ENV LD_LIBRARY_PATH /opt/gcc-13.2.0/lib:/opt/gcc-13.2.0/lib64:$LD_LIBRARY_PATH

RUN git clone -b llvmorg-17.0.6 https://github.com/llvm/llvm-project.git \
		&& mkdir /tmp/llvm-project/build \
		&& cd /tmp/llvm-project/build \
		&& cmake -G "Ninja"  -DCMAKE_BUILD_TYPE="Release" \
                   -DCMAKE_C_COMPILER=/opt/gcc-13.2.0/bin/gcc \
                   -DCMAKE_CXX_COMPILER=/opt/gcc-13.2.0/bin/g++ \
                   -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;compiler-rt;libclc;lld;lldb;openmp;polly;pstl;mlir;flang;" \
				   -DLLVM_ENABLE_RUNTIMES="libcxx;libunwind;libcxxabi" \
                   -DCMAKE_INSTALL_PREFIX=/opt/llvm17.0.6 \
                   -DLLVM_ENABLE_ASSERTIONS=On \
                   ../llvm \
		&& cmake --build . \
		&& cmake --install . \
		&& mkdir -p /tmp/llvm-project/omp-build/install  \
		&& cd /tmp/llvm-project/omp-build \
		&& LD_LIBRARY_PATH=/usr/local/cuda/compat:${LD_LIBRARY_PATH} \
		   PATH=/opt/llvm17.0.6/bin:${PATH} \
		   CC=clang CXX=clang++ cmake -Wno-dev -G Ninja -DCMAKE_BUILD_TYPE=Release \
			-DLIBOMPTARGET_DEVICE_ARCHITECTURES=all \
		   -DLLVM_ENABLE_PROJECTS="clang;openmp" ../llvm \
		   -DCMAKE_INSTALL_PREFIX=./install \
	   	&& cmake --build . \
		&& cmake --install . \
		&& find /tmp/llvm-project/omp-build/install -name libomptarget* -exec cp {} /opt/llvm17.0.6/lib/ \; \
		&& rm -rf /tmp/llvm-project

ENV LD_LIBRARY_PATH=
RUN { \
        echo "/opt/llvm17.0.6/lib"; \
        echo "/opt/llvm17.0.6/lib/x86_64-unknown-linux-gnu"; \
        } >> /etc/ld.so.conf.d/20-llvm.conf

RUN { \
        echo "/opt/gcc-13.2.0/lib"; \
        echo "/opt/gcc-13.2.0/lib64"; \
        } >> /etc/ld.so.conf.d/10-gcc-13.conf
RUN ldconfig
ENV PATH /opt/llvm17.0.6/bin:${PATH}
ENV LIBRARY_PATH="/opt/llvm17.0.6/lib/x86_64-unknown-linux-gnu"
WORKDIR /root

COPY ./entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
CMD ["/bin/bash", "--login"]
