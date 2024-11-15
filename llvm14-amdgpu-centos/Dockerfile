FROM centos:8

RUN sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-Linux-*
RUN sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-Linux-*
RUN dnf -y update --exclude cuda* --nogpgcheck
RUN dnf -y install gcc gcc-c++
RUN dnf -y install wget
RUN dnf -y install git
RUN dnf -y install cmake
RUN dnf -y install elfutils-libelf-devel libffi-devel
RUN dnf -y --enablerepo=powertools install ninja-build
RUN dnf -y install python3
RUN dnf -y install bzip2 flex
COPY rocm.repo  /etc/yum.repos.d/rocm.repo
RUN dnf -y update && dnf -y install rocm-hip-libraries

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

RUN git clone -b llvmorg-14.0.6 https://github.com/llvm/llvm-project.git \
		&& mkdir /tmp/llvm-project/build \
		&& cd /tmp/llvm-project/build \
		&& cmake -G "Ninja"  -DCMAKE_BUILD_TYPE="Release" \
                   -DCMAKE_C_COMPILER=/opt/gcc-12.1.0/bin/gcc \
                   -DCMAKE_CXX_COMPILER=/opt/gcc-12.1.0/bin/g++ \
				   -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;compiler-rt;libclc;lld;lldb;openmp;polly;pstl;mlir;flang;" \
				   -DLLVM_ENABLE_RUNTIMES="libcxx;libunwind;libcxxabi" \
                   -DCMAKE_INSTALL_PREFIX=/opt/llvm14.0.6 \
                   -DLLVM_ENABLE_ASSERTIONS=On \
                   ../llvm \
		&& cmake --build . \
		&& cmake --install . \
        && mkdir -p /tmp/llvm-project/omp-build/install  \
        && cd /tmp/llvm-project/omp-build \
        && PATH=/opt/llvm14.0.6/bin:${PATH} \
           CC=clang CXX=clang++ cmake -Wno-dev -G Ninja -DCMAKE_BUILD_TYPE=Release \
		   -DLIBOMPTARGET_AMDGCN_GFXLIST="gfx700;gfx701;gfx801;gfx803;gfx805;gfx810;gfx900;gfx902;gfx904;gfx906;gfx908;gfx909;gfx90c;gfx1010;gfx1011;gfx1012;gfx1013;gfx1030;gfx1031;gfx1032;gfx1033;gfx1034;gfx1035" \
           -DLLVM_ENABLE_PROJECTS="clang;openmp" ../llvm \
           -DCMAKE_INSTALL_PREFIX=./install \
        && cmake --build . \
        && cmake --install . \
        && find /tmp/llvm-project/omp-build/install -name libomptarget* -exec cp {} /opt/llvm14.0.6/lib/ \; \
		&& rm -rf /tmp/llvm-project

ENV LD_LIBRARY_PATH=
RUN { \
        echo "/opt/llvm14.0.6/lib"; \
        echo "/opt/llvm14.0.6/lib/x86_64-unknown-linux-gnu"; \
        } >> /etc/ld.so.conf.d/20-llvm.conf

RUN { \
        echo "/opt/rocm/lib"; \
        echo "/opt/rocm/lib64"; \
        } >> /etc/ld.so.conf.d/20-rocm.conf
RUN { \
        echo "/opt/gcc-12.1.0/lib"; \
        echo "/opt/gcc-12.1.0/lib64"; \
        } >> /etc/ld.so.conf.d/10-gcc-12.conf
RUN ldconfig
ENV LIBRARY_PATH="/opt/llvm14.0.6/lib/x86_64-unknown-linux-gnu;/opt/rocm/lib"
ENV PATH /opt/llvm14.0.6/bin:/opt/rocm/bin:${PATH}
WORKDIR /root