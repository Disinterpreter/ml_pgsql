# Postgre.SQL module for MTA-SA
## Build instructions

### Linux
Note: This guide will use Arch Linux as an example. For Red Hat/RHEL based systems, check our Dockerfile.


Install premake5, build tools and pgsql libs
```
sudo pacman -S base-devel premake postgresql-libs
```

Compile
```
premake5 gmake2
cd build
make
```

### Windows
Note: This guide assumes you have scoop installed. If you do not, then install it [here](https://scoop.sh).

Install premake5 and GNU make
```
scoop install premake make
```

Install tdm-gcc, or VS 2019
```
TDM-GCC: https://github.com/jmeubank/tdm-gcc/releases/download/v9.2.0-tdm64-1/tdm64-gcc-9.2.0.exe
VS 2019: Microsoft's website.
```

Compile
```
# for gcc/mingw
premake5 gmake2
cd build
make

# for vs2019
premake5 vs2019
cd build
explorer .
```