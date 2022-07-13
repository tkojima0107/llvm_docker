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