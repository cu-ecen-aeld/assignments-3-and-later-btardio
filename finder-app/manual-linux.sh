#!/bin/bash
# Script outline to install and build kernel.
# Author: Siddhant Jajoo.

set -e
set -u

OUTDIR=/tmp/aeld
#OUTDIR=$(pwd)/tmp/aeld
KERNEL_REPO=git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
KERNEL_VERSION=v5.15.163
BUSYBOX_VERSION=1_33_1
FINDER_APP_DIR=$(realpath $(dirname $0))
ARCH=arm64
CROSS_COMPILE=aarch64-none-linux-gnu-
MYPWD=$(pwd)

if [ $# -lt 1 ]
then
	echo "Using default directory ${OUTDIR} for output"
else
	OUTDIR=$1
	echo "Using passed directory ${OUTDIR} for output"
fi

mkdir -p ${OUTDIR}

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/linux-stable" ]; then
    #Clone only if the repository does not exist.
	echo "CLONING GIT LINUX STABLE VERSION ${KERNEL_VERSION} IN ${OUTDIR}"
	git clone ${KERNEL_REPO} --depth 1 --single-branch --branch ${KERNEL_VERSION}
fi
if [ ! -e ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ]; then
    cd linux-stable
    echo "Checking out version ${KERNEL_VERSION}"
    git checkout ${KERNEL_VERSION}

#    CXX=/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc
#    export CXX
    
#    make clean

#    make defconfig
    
#    make
    make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- mrproper

    make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- defconfig

    make -j8 ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- all


    make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- modules

    make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- dtbs

    #make



    # TODO: Add your kernel build steps here
fi

echo "Adding the Image in outdir"

echo "Creating the staging directory for the root filesystem"
cd "$OUTDIR"
if [ -d "${OUTDIR}/rootfs" ]
then
	echo "Deleting rootfs directory at ${OUTDIR}/rootfs and starting over"
    # NOTE: previous sudo
    rm  -rf ${OUTDIR}/rootfs
fi

# TODO: Create necessary base directories

mkdir -p $OUTDIR/rootfs
mkdir -p $OUTDIR/rootfs/bin
mkdir -p $OUTDIR/rootfs/dev
mkdir -p $OUTDIR/rootfs/etc
mkdir -p $OUTDIR/rootfs/home
mkdir -p $OUTDIR/rootfs/lib
mkdir -p $OUTDIR/rootfs/lib64
mkdir -p $OUTDIR/rootfs/proc
mkdir -p $OUTDIR/rootfs/sbin
mkdir -p $OUTDIR/rootfs/sys
mkdir -p $OUTDIR/rootfs/tmp
mkdir -p $OUTDIR/rootfs/usr
mkdir -p $OUTDIR/rootfs/usr/bin
mkdir -p $OUTDIR/rootfs/usr/lib
mkdir -p $OUTDIR/rootfs/usr/sbin
mkdir -p $OUTDIR/rootfs/var
mkdir -p $OUTDIR/rootfs/var/log

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/busybox" ]
then
    git clone git://busybox.net/busybox.git
    # git clone https://github.com/mirror/busybox.git
    
    cd busybox
    git checkout ${BUSYBOX_VERSION}
    # TODO:  Configure busybox

    make distclean
    make defconfig
    make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE}
    make CONFIG_PREFIX=${OUTDIR}/rootfs ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} install
    
else
    cd busybox
    make CONFIG_PREFIX=${OUTDIR}/rootfs ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} install
fi

# TODO: Make and install busybox


cp -r ${MYPWD}/lib_dependencies/* $OUTDIR/rootfs/

echo "Library dependencies"

# NOTE: bin/busybox previously
#${CROSS_COMPILE}readelf -a ${OUTDIR}/busybox/busybox | grep "program interpreter"
#${CROSS_COMPILE}readelf -a ${OUTDIR}/busybox/busybox | grep "Shared library"



#${CROSS_COMPILE}readelf -a ${OUTDIR}/bin/busybox | grep "program interpreter"
#${CROSS_COMPILE}readelf -a ${OUTDIR}/bin/busybox | grep "Shared library"

mknod -m 666 $OUTDIR/rootfs/dev/null c 1 3

mknod -m 666 $OUTDIR/rootfs/dev/console c 5 1

cd "${OUTDIR}/rootfs"

cp ${OUTDIR}/linux-stable/arch/arm64/boot/Image ${OUTDIR}

chown -R nobody:4 ${OUTDIR}/busybox

chown -R nobody:4 ${OUTDIR}/linux-stable

mkdir -p ${OUTDIR}/rootfs/home/conf
cp ${MYPWD}/finder.sh ${OUTDIR}/rootfs/home/
cp ${MYPWD}/finder-test.sh ${OUTDIR}/rootfs/home/
#cp ${MYPWD}/writer ${OUTDIR}/rootfs/home/
cp ${MYPWD}/conf/assignment.txt ${OUTDIR}/rootfs/home/conf/
cp ${MYPWD}/conf/username.txt ${OUTDIR}/rootfs/home/conf/
cp ${MYPWD}/autorun-qemu.sh ${OUTDIR}/rootfs/home/


#find . | cpio -H newc -ov --owner root:root > ${OUTDIR}/initramfs.cpio

#gzip -c -f ${OUTDIR}/initramfs.cpio > ${OUTDIR}/initramfs.cpio.gz

bash -c "cd ${MYPWD} && make CROSS_COMPILE=aarch64-none-linux-gnu-"

cp ${MYPWD}/writer ${OUTDIR}/rootfs/home/

find . | cpio -H newc -ov --owner root:root > ${OUTDIR}/initramfs.cpio

gzip -c -f ${OUTDIR}/initramfs.cpio > ${OUTDIR}/initramfs.cpio.gz

#cp initramfs.cpio.gz ${OUTDIR}/

# TODO: Add library dependencies to rootfs

# TODO: Make device nodes

# TODO: Clean and build the writer utility

# TODO: Copy the finder related scripts and executables to the /home directory
# on the target rootfs

# TODO: Chown the root directory

# TODO: Create initramfs.cpio.gz
