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
RUN dnf -y install ncurses-compat-libs
WORKDIR /tmp

	
RUN git clone -b release_12x https://github.com/flang-compiler/classic-flang-llvm-project.git \
 		&& mkdir /tmp/classic-flang-llvm-project/build \
 		&& cd /tmp/classic-flang-llvm-project/build \
 		&& cmake -G "Ninja"  -DCMAKE_BUILD_TYPE="Release" \
                    -DCMAKE_C_COMPILER=gcc \
                    -DCMAKE_CXX_COMPILER=g++  \
                    -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;compiler-rt;libclc;libcxx;libcxxabi;libunwind;lld;lldb;openmp;parallel-libs;polly;pstl;mlir;" \
                    -DCMAKE_INSTALL_PREFIX=/opt/llvm12.0.1 \
					-DLLVM_ENABLE_CLASSIC_FLANG=ON \
                    -DLLVM_ENABLE_ASSERTIONS=On \
                    ../llvm \
 		&& cmake --build . \
 		&& cmake --install . \
 		&& rm -rf /tmp/classic-flang-llvm-project


RUN wget https://github.com/flang-compiler/flang/releases/download/flang_20190329/flang-20190329-x86-70.tgz \
	&& mkdir -p /tmp/flang \
	&& tar xvf flang-20190329-x86-70.tgz -C /tmp/flang \
	&& git clone -b flang_20210324 https://github.com/flang-compiler/flang.git flang_src \
	&& mkdir -p /tmp/flang_src/runtime/libpgmath/build \
	&& cd /tmp/flang_src/runtime/libpgmath/build \
	&& export LD_LIBRARY_PATH=/tmp/flang/lib \
	&& cmake -DCMAKE_INSTALL_PREFIX=/opt/llvm12.0.1 \
               -DCMAKE_CXX_COMPILER=g++ \
               -DCMAKE_C_COMPILER=gcc .. \
	&& make -j `nproc` \
	&& make install \
	&& mkdir -p /tmp/flang_src/build \
	&& cd /tmp/flang_src/build \
	&& cmake -DCMAKE_INSTALL_PREFIX=/opt/llvm12.0.1 \
               -DLLVM_CONFIG=/opt/llvm12.0.1/bin/llvm-config \
               -DCMAKE_CXX_COMPILER=g++ \
               -DCMAKE_C_COMPILER=gcc \
               -DCMAKE_Fortran_COMPILER=/tmp/flang/bin/flang  \
               -DCMAKE_Fortran_COMPILER_ID=Flang \
    		   -DFLANG_LLVM_EXTENSIONS=ON \
			   -DWITH_WERROR=OFF .. \
	&& make -j `nproc` \
	&& make install \
	&& rm -rf /tmp/flang*

RUN echo "/opt/llvm12.0.1/lib" >> /etc/ld.so.conf.d/10-llvm.conf
RUN ldconfig
ENV PATH /opt/llvm12.0.1/bin:${PATH}
WORKDIR /root

CMD ["/bin/bash", "--login"]