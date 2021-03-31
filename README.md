# Effect of Monotonic Filtering on Collection Dynamics

Description to be added.
For any issues with reproducing the experiments, please contact [Giorgio Audrito](mailto:giorgio.audrito@unito.it).

## References

- FCPP main website: [https://fcpp.github.io](https://fcpp.github.io).
- FCPP documentation: [http://fcpp-doc.surge.sh](http://fcpp-doc.surge.sh).
- FCPP sources: [https://github.com/fcpp/fcpp](https://github.com/fcpp/fcpp).


## Batch Experiments

### Vagrant

Download Vagrant from [https://www.vagrantup.com](https://www.vagrantup.com), then type the following commands in a terminal:
```
vagrant up
vagrant ssh
cd fcpp
./make.sh run -O batch
```
Then you should get output about building the experiments and running them (in the Vagrant virtual machine). After that you can exit and stop the virtual machine through:
```
exit
vagrant halt
```

### Docker

Download Docker from [https://www.docker.com](https://www.docker.com), then download the Docker container from GitHub by typing the following command in a terminal:
```
docker pull docker.pkg.github.com/fcpp/fcpp/container:1.0
```
Alternatively, you can build the container yourself with the following command:
```
docker build -t docker.pkg.github.com/fcpp/fcpp/container:1.0 .
```
Once you have the Docker container locally available, type the following commands:
```
docker run -it --volume $PWD:/fcpp --workdir /fcpp docker.pkg.github.com/fcpp/fcpp/container:1.0 bash
./make.sh run -O batch
```
Then you should get output about building the experiments and running them (in the Docker container). After that you can exit and stop the container through:
```
exit
```

### Custom Build

In order to get started on your machine you need the following installed:

- [Bazel](https://bazel.build) (tested with version 2.1.0)
- [GCC](https://gcc.gnu.org) (tested with version 9.3.0) or [Clang](https://clang.llvm.org) (tested with version 12.0.0)
- [Asymptote](http://asymptote.sourceforge.io) (for building the plots)

Once you have them installed, you should be able to run `./make.sh run -O batch`, getting output about building the experiments and running them.


## Interactive Experiment

### Virtual Machines

The simulations in this repository have an OpenGL-based graphical interface. Common Virtual Machine software (e.g., VirtualBox) has faulty support for OpenGL, hence running the experiments in a VM is not supported. Based on preliminary testing, the simulations may not start on some VMs, while starting on others with graphical distortions (e.g., limited colors).

### Windows

Pre-requisites:
- [Git Bash](https://gitforwindows.org) (for issuing unix-style commands)
- [MinGW-w64 builds 8.1.0](http://mingw-w64.org/doku.php/download/mingw-builds)
- [CMake 3.9](https://cmake.org) (or higher)
- [Asymptote](http://asymptote.sourceforge.io) (for building the plots)

During CMake installation, make sure you select to add `cmake` to the `PATH` (at least for the current user).
During MinGW installation, make sure you select "posix" threads (should be the default) and not "win32" threads. After installing MinGW, you need to add its path to the environment variable `PATH`. The default path should be:
```
C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin
```
but the actual path may vary depending on your installation.

Clone this repository, then go into its main directory to launch the `make.sh` script:
```
./make.sh gui windows monotonic_filtering
```
You should see output about building the executables, then the graphical simulation should pop up. When the simulation closes, the resulting data will be plotted in folder `output/`.

### Linux

Pre-requisites:
- Xorg-dev package (X11)
- G++ 9 (or higher)
- CMake 3.9 (or higher)
- Asymptote (for building the plots)

To install these packages in Ubuntu, type the following command:
```
sudo apt-get install xorg-dev g++ cmake asymptote
```

Clone this repository, then go into its main directory to launch the `make.sh` script:
```
./make.sh gui unix monotonic_filtering
```
You should see output about building the executables, then the graphical simulation should pop up. When the simulation closes, the resulting data will be plotted in folder `output/`.

### MacOS

Pre-requisites:
- Xcode Command Line Tools
- CMake 3.9 (or higher)
- [Asymptote](http://asymptote.sourceforge.io) (for building the plots)

To install them, assuming you have the [brew](https://brew.sh) package manager, type the following commands:
```
xcode-select --install
brew install cmake asymptote
```

Clone this repository, then go into its main directory to launch the `make.sh` script:
```
./make.sh gui unix monotonic_filtering
```
You should see output about building the executables, then the graphical simulation should pop up. When the simulation closes, the resulting data will be plotted in folder `output/`.

### GUI Controls

To launch the experiment manually, move to the `bin` directory and run the `monotonic_filtering` executable. This will open a window displaying the simulation scenario, initially still: you can start running the simulation by pressing `P` (current simulated time is displayed in the bottom-left corner). While the simulation is running, network statistics will be periodically printed in the console, and aggregated in form of an Asymptote plot at simulation end. You can interact with the simulation through the following keys:
- `Esc` to end the simulation
- `P` to stop/resume
- `O`/`I` to speed-up/slow-down simulated time
- `L` to show/hide connection links between nodes
- `G` to show/hide the grid on the reference plane
- `M` to toggle a marker enabling to select nodes for additional info
- `C` resets the camera to the starting position
- `Q`,`W`,`E`,`A`,`S`,`D` to move the simulation area along orthogonal axes
- `right-click`+`mouse drag` to rotate the camera
- `mouse scroll` for zooming in and out
-`left-shift` added to the camera commands above for precision control
The source node at every time is highlighted as a larger cube. Every node is colored with two colors:
- the color in the central part of each node represents the simple collection estimate for that node;
- the color in the two lateral sides of each node represents the collection estimate with monotonic filtering for that node.
Colors vary from red (estimate of only one node) to magenta (correct estimate) to black (infinitely large overestimate).
