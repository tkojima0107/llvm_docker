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
WORKDIR /tmp
RUN wget https://github.com/gcc-mirror/gcc/archive/refs/tags/releases/gcc-13.3.0.tar.gz \
	&& tar xvf gcc-13.3.0.tar.gz \
	&& cd gcc-releases-gcc-13.3.0 \
	&& ./contrib/download_prerequisites \
	&& mkdir build \
	&& cd build \
	&& ../configure --prefix=/opt/gcc-13.3.0/ --enable-languages=c,c++ --disable-multilib \
	&& make -j`nproc` \
	&& make install \
	&& rm -rf /tmp/gcc-*

ENV LD_LIBRARY_PATH /opt/gcc-13.3.0/lib:/opt/gcc-13.3.0/lib64:$LD_LIBRARY_PATH

RUN git clone -b llvmorg-18.1.8 https://github.com/llvm/llvm-project.git \
		&& mkdir /tmp/llvm-project/build \
		&& cd /tmp/llvm-project/build \
		&& cmake -G "Ninja"  -DCMAKE_BUILD_TYPE="Release" \
                   -DCMAKE_C_COMPILER=/opt/gcc-13.3.0/bin/gcc \
                   -DCMAKE_CXX_COMPILER=/opt/gcc-13.3.0/bin/g++ \
                   -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;compiler-rt;libclc;lld;lldb;openmp;polly;pstl;mlir;flang;" \
				   -DLLVM_ENABLE_RUNTIMES="libcxx;libunwind;libcxxabi" \
                   -DCMAKE_INSTALL_PREFIX=/opt/llvm18.1.8 \
                   -DLLVM_ENABLE_ASSERTIONS=On \
                   ../llvm \
		&& cmake --build . \
		&& cmake --install . \
		&& cd / \
		&& rm -rf /tmp/llvm-project

ENV LD_LIBRARY_PATH=
RUN { \
        echo "/opt/llvm18.1.8/lib"; \
        echo "/opt/llvm18.1.8/lib/x86_64-unknown-linux-gnu"; \
        } >> /etc/ld.so.conf.d/20-llvm.conf

RUN { \
        echo "/opt/gcc-13.3.0/lib"; \
        echo "/opt/gcc-13.3.0/lib64"; \
        } >> /etc/ld.so.conf.d/10-gcc-13.conf
RUN ldconfig
ENV PATH /opt/llvm18.1.8/bin:${PATH}
ENV LIBRARY_PATH="/opt/llvm18.1.8/lib/x86_64-unknown-linux-gnu"
WORKDIR /root

CMD ["/bin/bash", "--login"]
