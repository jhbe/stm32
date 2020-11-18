# Setup

    sudo apt install libusb-1.0-0-dev

Add the following file named /etc/udev/rules.d/60-usbtest.rules:

    ATTRS{idVendor}=="3456", ATTRS{idProduct}=="1150", MODE="664", GROUP="plugdev"

then

    sudo usermod -aG plugdev $USER

# Build

    cmake -B cmake-build-debug
    make -C cmake-build-debug

Add the following file named /etc/udev/rules.d/60-usbtest.rules:

    ATTRS{idVendor}=="3456", ATTRS{idProduct}=="1150", MODE="664", GROUP="plugdev"

then

    sudo usermod -aG plugdev $USER


