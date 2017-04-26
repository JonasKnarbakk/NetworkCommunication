# Minimum cmake version to use
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# Set the project name
PROJECT (NetworkCommunication)

# Add subdirectories that contain CMakeLists.txt
ADD_SUBDIRECTORY(Server)
ADD_SUBDIRECTORY(Client)

# Create som custom makefile commands to run the binaries easier
ADD_CUSTOM_TARGET(run-client) 
ADD_CUSTOM_COMMAND(TARGET run-client COMMAND ./bin/Client) 

ADD_CUSTOM_TARGET(run-server) 
ADD_CUSTOM_COMMAND(TARGET run-server COMMAND ./bin/Server) 
