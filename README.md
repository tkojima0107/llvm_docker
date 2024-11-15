# Docker images for LLVM
This repositoy has Dockerfiles in `llvm<VERSION>-<DISTRIBUTION>` folders.
The pre-built images can be pulled from Docker Hub.
```
docker pull tkojima0107/llvm:<VERSION>-<DISTRIBUTION>
```

## LLVM versions
* LLVM 12.0.1
* LLVM 13.0.1
* LLVM 14.0.6
* LLVM 15.0.7
* LLVM 16.0.6
* LLVM 17.0.6
* LLVM 18.1.8

## Base distribution
* CentOS 8 (except for nvidia images)
* Ubuntu 20.04
* Rocky Linux 8 (only for nvidia images)

The nvidia images based on CentOS 8 are no longer available.
Docker Hub repository currently keep `*-nvidia-centos8*` images, but they are deprecated.

## LLVM with OpenMP offloading libraries for NVIDIA GPUs
In addition to the above images, this repository has images based on Nvidia cuda images for OpenMP offloading.
So, nvidia-docker is needed to use them.

### Usage Example
For running a container,
```
docker run -it --runtime=nvidia tkojima0107/llvm:<VERSION>-nvidia-<DISTRIBUTION>
```

For compiling an application,
```
clang++ -O3 -Xopenmp-target -march=<GPU ARCH> -fopenmp -fopenmp-targets=nvptx64 <SOURCE FILES>
```

After LLVM 16, the following command also works:
```
clang++ -O3 --offload_arch=<GPU ARCH> -fopenmp <SOURCE FILES>
```

`<GPU ARCH>` is named *SM_XY*, where X and Y correspond to [compute capability](https://developer.nvidia.com/cuda-gpus) *X.Y*.

Available GPU architectures are follows:

| LLVM version | Supported CUDA version | Supported GPU architectures |
|:------------:|:-------------------:|:---------------------------|
| 12.0.1       | <=11.0[^1] | sm_35, sm_37, sm_50, sm_52, sm_53, sm_60, sm_61, sm_62, sm_70, sm_72, sm_75, sm_80 |
| 13.0.1       | <=11.2[^1] |  sm_35, sm_37, sm_50, sm_52, sm_53, sm_60, sm_61, sm_62, sm_70, sm_72, sm_75, sm_80 |
| 14.0.6       | <=11.5 | sm_35, sm_37, sm_50, sm_52, sm_53, sm_60, sm_61, sm_62, sm_70, sm_72, sm_75, sm_80, sm_86 |
| 15.0.7       | <=11.5 | sm_35, sm_37, sm_50, sm_52, sm_53, sm_60, sm_61, sm_62, sm_70, sm_72, sm_75, sm_80, sm_86 |
| 16.0.6       | <=11.5 | sm_35, sm_37, sm_50, sm_52, sm_53, sm_60, sm_61, sm_62, sm_70, sm_72, sm_75, sm_80, sm_86 (sm_89, sm_90)[^2]  |
| 17.0.6       | <=11.8 | sm_35, sm_37, sm_50, sm_52, sm_53, sm_60, sm_61, sm_62, sm_70, sm_72, sm_75, sm_80, sm_86, sm_89, sm_90 |
| 18.1.8       | <=12.3 | sm_35, sm_37, sm_50, sm_52, sm_53, sm_60, sm_61, sm_62, sm_70, sm_72, sm_75, sm_80, sm_86, sm_89, sm_90 |

[^1]: CUDA version later than 10.1 will emit a warning message.
[^2]: CUDA 11.5 is fully supported, but that version does not support sm_89 and sm_90.

## LLVM with OpenMP offloading libraries for AMD GPUs
This repository also has images for OpenMP offloading with AMD GPU.
In each image, [ROCm toolkit](https://rocmdocs.amd.com/en/latest/) is installed.

LLVM 15 or later versions changed the default compiler driver for OpenMP offloading to `openmp-new-driver`.
The new driver no longer needs bitcode library built from the llvm source tree.
Instead, the new driver requires ROCm device library for linking.
Thus, the Dockerfile for the versions later than LLVM 15 does not build the bitcode library.
However, LLVM 15 can still use the legacy driver by specifying `-fno-openmp-new-driver` option so that building the bitcode library is kept in its image.

### Known issues with LLVM 15-18
If multiple and different GPUs are installed in the host machine,
the OpenMP offloading library might regards compiled binary as an incompatible one even if specifying the correct GPU architecture.
It can be solved by setting `ROCR_VISIBLE_DEVICES` environment variable to show only the target GPU to the library.

This issue does not occur with LLVM in ROCm developed by AMD.
LLVM 19 seems to have fixed this issue.


### Usage Example
For running a container,
```
docker run -it --device=/dev/kfd --device=/dev/dri --security-opt seccomp=unconfined --group-add video tkojima0107/llvm:<VERSION>-amdgpu-<DISTRIBUTION>
```

For compiling an application,
```
clang++ -O3 -Xopenmp-target -march=<GPU ARCH> -fopenmp -fopenmp-targets=amdgcn-amd-amdhsa <SOURCE FILES>
```

After LLVM 16, the following command also works:
```
clang++ -O3 --offload-arch=<GPU ARCH> -fopenmp <SOURCE FILES>
```

Available GPU architectures are follows:

| LLVM version | Supported GPU architectures |
|:------------:|:---------------------------|
| 13.0.1       | gfx700, gfx701, gfx801, gfx803, gfx805, gfx810, gfx900, gfx902, gfx904, gfx906, gfx908|
| 14.0.6       | gfx700, gfx701, gfx801, gfx803, gfx805, gfx810, gfx900, gfx902, gfx904, gfx906, gfx908, gfx909, gfx90c, gfx1010, gfx1011, gfx1012, gfx1013, gfx1030, gfx1031|
| 15.0.7       | gfx700, gfx701, gfx801, gfx803, gfx805, gfx810, gfx900, gfx902, gfx904, gfx906, gfx908, gfx909, gfx90c, gfx1010, gfx1011, gfx1012, gfx1013, gfx1030, gfx1031, gfx1032, gfx1033, gfx1034, gfx1035, gfx1036 |
| 16.0.6       | gfx700, gfx701, gfx801, gfx803, gfx805, gfx810, gfx900, gfx902, gfx904, gfx906, gfx908, gfx909, gfx90c, gfx1010, gfx1011, gfx1012, gfx1013, gfx1030, gfx1031, gfx1032, gfx1033, gfx1034, gfx1035, gfx1036, gfx1100, gfx1101, gfx1102, gfx1103 |
| 17.0.6       |  gfx700, gfx701, gfx801, gfx803, gfx900, gfx902, gfx906, gfx908, gfx90a, gfx90c, gfx940, gfx1010, gfx1030, gfx1031, gfx1032, gfx1033, gfx1034, gfx1035, gfx1036, gfx1100, gfx1101, gfx1102, gfx1103, gfx1150, gfx1151|
| 18.1.8       | gfx700, gfx701, gfx801, gfx803, gfx900, gfx902, gfx906, gfx908, gfx90a, gfx90c, gfx940, gfx941, gfx942, gfx1010, gfx1030, gfx1031, gfx1032, gfx1033, gfx1034, gfx1035, gfx1036, gfx1100, gfx1101, gfx1102, gfx1103, gfx1150, gfx1151 |


The following command will tell you which architecture your GPU is:
```
$ rocminfo | grep gfx
```

## Sample code for OpenMP offloading
See [openmp_exmaples](openmp_examples)

## Other images
* `llvm12-flang-centos`
LLVM12 with [flang](https://github.com/flang-compiler/flang), a fortran compiler.

Starting with LLVM 15, `flang-new` command is available for Fortran code generation, which can be built without any externel projects.

