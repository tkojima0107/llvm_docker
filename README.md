# Docker images for LLVM
This repositoy has Dockerfiles in `llvm<VERSION>-<DISTRIBUTION>` folders.
The pre-built images can be pull from Docker Hub.
```
docker pull tkojima0107/llvm<VERSION>-<DISTRIBUTION>
```

## LLVM versions
* LLVM 12.0.1
* LLVM 13.0.1
* LLVM 14.0.6

## Base distribution
* CentOS 8
* Ubuntu 20.04


## LLVM with OpenMP offloading libraries for NVIDIA GPUs
In addition to the above images, this repository has images based on Nvidia cuda images for OpenMP offloading.
So, nvidia-docker is needed to use them.

### Usage Example
For running a container,
```
docker run -it --runtime=nvidia tkojima0107/llvm<VERSION>-nvidia<DISTRIBUTION>
```

For compiling an application,
```
clang++ -O3 -Xopenmp-target -march=<GPU ARCH> -fopenmp -fopenmp-targets=nvptx64 --cuda-path=/usr/local/cuda-11.2 <SOURCE FILES>
```

`<GPU ARCH>` is named *SM_XY*, where X and Y correspond to [compute capability](https://developer.nvidia.com/cuda-gpus) *X.Y*.

Available GPU architectures are follows:

sm_35, sm_37, sm_50, sm_52, sm_53, sm_60, sm_61, sm_62, sm_70, sm_72, sm_75, sm_80, sm_86

Note: sm_86 is supported from LLVM 14


## LLVM with OpenMP offloading libraries for AMD GPUs
This repository also has images for OpenMP offloading with AMD GPU.
In each image, [ROCm toolkit](https://rocmdocs.amd.com/en/latest/) is installed.

### Usage Example
For running a container,
```
docker run -it --device=/dev/kfd --device=/dev/dri --security-opt seccomp=unconfined --group-add video tkojima0107/llvm<VERSION>-amdgpu-<DISTRIBUTION>
```

For compiling an application,
```
clang++ -O3 -Xopenmp-target=amdgcn-amd-amdhsa -march=<GPU ARCH> -fopenmp -fopenmp-targets=amdgcn-amd-amdhsa <SOURCE FILES>
```

Available GPU architectures

gfx700 gfx701 gfx801 gfx803 gfx805 gfx810 gfx900 gfx902 gfx904 gfx906 gfx908 gfx909 gfx90c gfx1010 gfx1011 gfx1012 gfx1013 gfx1030 gfx1031 gfx1032 gfx1033 gfx1034 gfx1035


The following command will tell you which architecture your GPU is:
```
$ rocminfo | grep gfx
```

## Other images
* `llvm12-flang-centos`
LLVM12 with [flang](https://github.com/flang-compiler/flang), a fortran compiler.

