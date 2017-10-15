# gateway-luna
## Settings of qtcreator to build the project
1. Configure raspberry kit with yocto toolchain
2. In projects->build settings, add the following flags:
```
CMAKE_SYSROOT:STRING /opt/poky/2.3/metaqtsdk/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi
CMAKE_CXX_COMPILER_WORKS:STRING=1
CMAKE_C_COMPILER_WORKS:STRING=1
```

