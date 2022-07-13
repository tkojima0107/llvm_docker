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
WORKDIR /tmp
RUN git clone -b llvmorg-12.0.1 https://github.com/llvm/llvm-project.git \
		&& mkdir /tmp/llvm-project/build \
		&& cd /tmp/llvm-project/build \
		&& cmake -G "Ninja"  -DCMAKE_BUILD_TYPE="Release" \
                   -DCMAKE_C_COMPILER=gcc \
                   -DCMAKE_CXX_COMPILER=g++  \
                   -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;compiler-rt;libclc;libcxx;libcxxabi;libunwind;lld;lldb;openmp;parallel-libs;polly;pstl;mlir;flang;" \
                   -DCMAKE_INSTALL_PREFIX=/opt/llvm12.0.1 \
                   -DLLVM_ENABLE_ASSERTIONS=On \
                   -DLLVM_ENABLE_CLASSIC_FLANG=ON \
                   ../llvm \
		&& cmake --build . \
		&& cmake --install . \
		&& cd / \
		&& rm -rf /tmp/llvm-project

RUN echo 'export LD_LIBRARY_PATH=/opt/llvm12.0.1/lib:${LD_LIBRARY_PATH}' >> /etc/profile
RUN echo 'export PATH=/opt/llvm12.0.1/bin:${PATH}' >> /etc/profile
WORKDIR /root

CMD ["/bin/bash", "--login"]