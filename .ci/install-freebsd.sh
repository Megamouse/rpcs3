#!/usr/bin/env -S su -m root -ex
# NOTE: this script is run under root permissions
# shellcheck shell=sh disable=SC2096

# RPCS3 often needs recent Qt and Vulkan-Headers
sed -i '' 's/quarterly/latest/' /etc/pkg/FreeBSD.conf

export ASSUME_ALWAYS_YES=true
pkg info # debug

# Prefer newer Clang than in base system (see also .ci/build-freebsd.sh)
pkg install llvm13

# Mandatory dependencies (qt6-dbus and qt6-gui are pulled via qt6-widgets)
pkg install git ccache cmake ninja qt6-qmake qt6-buildtools qt6-widgets qt6-concurrent qt6-multimedia qt6-svg glew openal-soft ffmpeg

# Optional dependencies (libevdev is pulled by qt6-gui)
pkg install pkgconf alsa-lib pulseaudio sdl2 evdev-proto vulkan-headers vulkan-loader
