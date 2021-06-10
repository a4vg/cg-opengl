defaults write org.xquartz.X11 enable_iglx -bool true
xhost + `hostname`

docker run -it --rm \
    -v $(pwd):/$(pwd) \
    -w $(pwd) \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
        -e DISPLAY=`hostname`:0 \
    --privileged \
    graphics:latest
