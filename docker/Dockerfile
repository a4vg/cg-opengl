FROM nvidia/opengl:1.0-glvnd-devel-ubuntu20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
     cmake \
     vim \
     build-essential \
     libglew-dev \
     freeglut3-dev

CMD ["/bin/bash"]
