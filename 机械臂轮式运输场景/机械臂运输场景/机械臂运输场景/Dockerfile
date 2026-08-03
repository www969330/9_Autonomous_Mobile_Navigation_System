FROM palroboticssl/tiago_tutorials:melodic

ENV DEBIAN_FRONTEND=noninteractive
ENV ROS_DISTRO=melodic

# Setup catkin workspace
WORKDIR /catkin_ws
RUN mkdir -p src

# Copy project source
COPY . /catkin_ws/src/decluttering_domestic_robot/

# Copy world file to tiago_gazebo worlds directory
RUN mkdir -p /tiago_public_ws/src/tiago_simulation/tiago_gazebo/worlds && \
    cp /catkin_ws/src/decluttering_domestic_robot/world/smallHouse.world /tiago_public_ws/src/pal_gazebo_worlds/worlds/

# Source ROS and Tiago workspace, then build
RUN /bin/bash -c "source /opt/ros/melodic/setup.bash && source /tiago_public_ws/devel/setup.bash && cd /catkin_ws && catkin build"

# Add to bashrc (source Tiago workspace first, then our workspace)
RUN echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc && \
    echo "source /tiago_public_ws/devel/setup.bash" >> ~/.bashrc && \
    echo "source /catkin_ws/devel/setup.bash" >> ~/.bashrc

WORKDIR /catkin_ws

# Override base image's entrypoint to avoid 'bash /bin/bash' issue
ENTRYPOINT []
CMD ["/bin/bash"]
