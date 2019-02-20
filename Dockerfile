FROM ubuntu:18.04

RUN apt-get -y update && \
    DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends --yes \
        autogen \
        automake \
        bash \
        bc \
        bison \
        build-essential \
        bzip2 \
        ca-certificates \
        cmake \
        curl \
        file \
        flex \
        gettext \
        git \
        glew-utils \
        gzip \
        libglew-dev \
        libglew2.0 \
        libglew-dev \
        libtool \
        libx11-dev \
        llvm \
        llvm-dev \
        make \
        menu \
        mesa-common-dev \
        mesa-utils \
        ncurses-dev \
        net-tools \
        openbox \
        pax \
        pkg-config \
        python \
        python-pip \
        python-pip \
        rsync \
        sed \
        software-properties-common \
        ssh \
        sudo \
        supervisor \
        tar \
        tint2 \
        vim \
        websockify \
        wget \
        x11-xserver-utils \
        x11vnc \
        xinit \
        xorg-dev \
        xserver-xorg-input-void \
        xserver-xorg-video-dummy \
        xterm \
        xz-utils \
        zip \
        zlib1g-dev && \
    rm -f /usr/share/applications/x11vnc.desktop && \
    apt-get remove -y python-pip && \
    wget https://bootstrap.pypa.io/get-pip.py && \
    python get-pip.py && \
    pip install supervisor-stdout && \
    apt-get -y clean && \
    apt-get -y autoremove

RUN git clone https://github.com/kanaka/noVNC.git /opt/noVNC && \
    cd /opt/noVNC && \
    git checkout 6a90803feb124791960e3962e328aa3cfb729aeb && \
    ln -s vnc_auto.html index.html

# noVNC (http server) is on 6080, and the VNC server is on 5900
EXPOSE 6080 5900

COPY etc/skel/.xinitrc /etc/skel/.xinitrc

RUN useradd -m -s /bin/bash user
USER user
RUN cp /etc/skel/.xinitrc /home/user/
USER root

RUN echo "user ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/user
RUN echo "root ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/root

COPY etc /etc
COPY usr /usr

ENV DISPLAY :0

RUN git clone https://github.com/glfw/glfw.git /src/glfw.git && \
    cd /src/glfw.git && \
    git checkout c90c7b97109db909577e3bf540b5f884422b7e14 && \
    cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DBUILD_SHARED_LIBS=ON . && \
    make all install

# Add RIT makemake to image; build it
COPY rit/makemake /root/makemake
RUN cd /root/makemake && \
    make all && \
    cp makemake /usr/bin/makemake && \
    cp makemake /usr/bin/gmakemake && \
    chmod a+x /usr/bin/makemake && \
    chmod a+x /usr/bin/gmakemake

COPY labs /home/user/labs
COPY rit /home/user/rit

RUN find /home/user/rit -type d -exec chown user {} \; && \
    find /home/user/rit -type f -exec chown user {} \; && \
    find /home/user/rit -type d -exec chmod u+wx {} \; && \
    find /home/user/rit -type f -exec chmod u+wr {} \; && \
    find /home/user/labs -type d -exec chown user {} \; && \
    find /home/user/labs -type f -exec chown user {} \; && \
    find /home/user/labs -type d -exec chmod u+wx {} \; && \
    find /home/user/labs -type f -exec chmod u+wr {} \; && \
    chmod a+x /home/user/rit/startLabC.sh && \
    chmod a+x /home/user/rit/startLabC++.sh

WORKDIR /root
CMD ["/usr/bin/supervisord", "-c", "/etc/supervisor/supervisord.conf"]

ENV LIBGL_ALWAYS_SOFTWARE true
ENV MESA_GL_VERSION_OVERRIDE 3.2COMPAT
# ENV APP "xterm -e /home/user/rit/startLabC++.sh"
ENV APP "xterm"