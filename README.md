# CS3460_Project - Live Network Flow Monitor
We have selected option 2, Live Network Flow Monitor, for our project.

## Team Members
- Jayne Auger (**TEAM LEADER**)
	- A02380810
	- a02380810@usu.edu

- Emberly Walburger
	- A02360714
	- a02360714@aggies.usu.edu

- Gannon O'Leary
	- A02417113
	- a02417113@aggies.usu.edu

## Build Instructions

You can choose to use the build script or the manual build instructions.

### Install Dependencies

`sudo apt-get install -y build-essential cmake pkg-config libpcap-dev`

### Using the Build Script

- run the command `./build.sh` in the root of the directory to build and configure the project

> [!Note] You may need to run `chmod +x build.sh` to make build.sh into an executable

### Manual Build Instructions

- run the command `cmake -S . -B build` in the root of the directory to configure the project
- run `cmake --build build` while in the main directory to build the project

## Run Instructions

run this command in the main directory `sudo ./build/flow_monitor` and it should print interface options

- sudo is important in this command because capturing network packets using libpcap requires root or administrator privileges
