# Cpp Http Interceptor

## Dependencies

Please, **before using this program**, install the [Conan](https://conan.io/) package manager and the [CMake](https://cmake.org/) build tool.
In-code dependencies will be installed later using `Conan`.

1. The [Conan](https://conan.io/) package manager.
2. [CMake](https://cmake.org/) as building tool.
3. [PcapPlusPlus](https://pcapplusplus.github.io/) for network packets capture.
4. [spdlog](https://github.com/gabime/spdlog) for fast logging.
5. [TCLAP](https://tclap.sourceforge.net/) for command line argument parsing.

### Windows Dependencies

If you're running a Windows machine, make sure to install a Pcap API implementation such as
[NpCap](https://npcap.com/#download) (recommended) or [WinPcap](https://www.winpcap.org/install/default.htm) (deprecated).

## Build

```shell
# 1. Clone the project to your computer
git clone https://github.com/pr1s10n3r/CppHttpInterceptor.git

# 2. Create a build directory and enter it
mkdir build && cd build

# 3. Install dependencies using the Conan package manager
conan install .. --build=missing

# 4. Generate CMake files.
cmake .. -G "Unix Makefiles"
# 4.1 If you are on Windows, generate CMake files using Visual Studio 17 2022 generator
cmake .. -G "Visual Studio 17 2022"

# 5. Compile project using generated make files
cmake --build . --config Release
```

> If you gave your build directory another name rather than `build`. **PLEASE DO NOT PUSH IT TO GITHUB**.

### Windows Build using Visual Studio

If for some reason you want to build the program using Visual Studio, make sure you're using the Release profile.

## Usage

If the build was successful, you should find an executable file named `chi` or `chi.exe` if you're on Windows inside the
`bin` library inside your `build` directory. You can use that executable in the following way:

```shell
# General program syntax:
chi \<-i INTERFACE_TO_INTERCEPT\> \<-o PATH_TO_OUTPUT_FILE\> \<-l TIME_IN_SECONDS\>

# For example, intercept the interface "en0" for 10 seconds and save the result to a file called output.txt
chi -i en0 -o output.txt -l 10
```

## Known Limitations

1. This program **does not support** `HTTPs` packets capture although its support should be easy to implement.
2. If you are on a Windows platform, it is recommended to build this program using Visual Studio.
3. For most of users, this program should be running as privileged user (`root` for friends).

## License

See the [LICENSE](./LICENSE) file.
