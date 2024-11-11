FROM nvidia/cuda:11.5.2-devel-rockylinux8

RUN dnf -y update --exclude cuda* --nogpgcheck
RUN dnf -y install gcc gcc-c++
RUN dnf -y install wget
RUN dnf -y install git
RUN dnf -y install cmake
RUN dnf -y install elfutils-libelf-devel libffi-devel
RUN dnf -y --enablerepo=powertools install ninja-build
RUN dnf -y install python3
RUN dnf -y install bzip2 flex

WORKDIR /tmp
RUN wget https://github.com/gcc-mirror/gcc/archive/refs/tags/releases/gcc-12.1.0.tar.gz \
	&& tar xvf gcc-12.1.0.tar.gz \
	&& cd gcc-releases-gcc-12.1.0 \
	&& ./contrib/download_prerequisites \
	&& mkdir build \
	&& cd build \
	&& ../configure --prefix=/opt/gcc-12.1.0/ --enable-languages=c,c++ --disable-multilib \
	&& make -j`nproc` \
	&& make install \
	&& rm -rf /tmp/gcc-*

ENV LD_LIBRARY_PATH /opt/gcc-12.1.0/lib:/opt/gcc-12.1.0/lib64:$LD_LIBRARY_PATH

RUN git clone -b llvmorg-15.0.7 https://github.com/llvm/llvm-project.git \
		&& mkdir /tmp/llvm-project/build \
		&& cd /tmp/llvm-project/build \
		&& cmake -G "Ninja"  -DCMAKE_BUILD_TYPE="Release" \
                   -DCMAKE_C_COMPILER=/opt/gcc-12.1.0/bin/gcc \
                   -DCMAKE_CXX_COMPILER=/opt/gcc-12.1.0/bin/g++ \
                   -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;compiler-rt;libclc;lld;lldb;openmp;polly;pstl;mlir;flang;" \
				   -DLLVM_ENABLE_RUNTIMES="libcxx;libunwind;libcxxabi" \
                   -DCMAKE_INSTALL_PREFIX=/opt/llvm15.0.7 \
                   -DLLVM_ENABLE_ASSERTIONS=On \
                   ../llvm \
		&& cmake --build . \
		&& cmake --install . \
		&& mkdir -p /tmp/llvm-project/omp-build/install  \
		&& cd /tmp/llvm-project/omp-build \
		&& LD_LIBRARY_PATH=/usr/local/cuda/compat:${LD_LIBRARY_PATH} \
		   PATH=/opt/llvm15.0.7/bin:${PATH} \
		   CC=clang CXX=clang++ cmake -Wno-dev -G Ninja -DCMAKE_BUILD_TYPE=Release \
		   -DCLANG_OPENMP_NVPTX_DEFAULT_ARCH=sm_86 -DLIBOMPTARGET_NVPTX_COMPUTE_CAPABILITIES=all \
		   -DLLVM_ENABLE_PROJECTS="clang;openmp" ../llvm \
		   -DCMAKE_INSTALL_PREFIX=./install \
	   	&& cmake --build . \
		&& cmake --install . \
		&& find /tmp/llvm-project/omp-build/install -name libomptarget* -exec cp {} /opt/llvm15.0.7/lib/ \; \
		&& rm -rf /tmp/llvm-project

ENV LD_LIBRARY_PATH=/opt/gcc-12.1.0/lib:/opt/gcc-12.1.0/lib64
RUN echo "/opt/llvm15.0.7/lib" >> /etc/ld.so.conf.d/20-llvm.conf
RUN { \
        echo "/opt/gcc-12.1.0/lib"; \
        echo "/opt/gcc-12.1.0/lib64"; \
        } >> /etc/ld.so.conf.d/10-gcc-12.conf
RUN rm /etc/ld.so.cache
ENV PATH /opt/llvm15.0.7/bin:${PATH}
WORKDIR /root

CMD ["/bin/bash", "--login"]
