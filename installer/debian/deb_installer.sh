#!/bin/sh

sudo apt-get install dpkg-dev dh-make debhelper devscripts pbuilder fakeroot dos2unix

DIST_DIR=.

rm -rf $DIST_DIR

mkdir $DIST_DIR

VERSION="$(cat $MADARA_ROOT/VERSION.txt)"

# because of silliness with the gnuace type for MPC, our MADARA
# libs are not actually created with the correct version. Instead,
# MPC creates a shared object with the ACE_VERSION + 1 minor version
# The following variables MUST BE CHANGED with each new ACE version used
ACE_VERSION=6.1.5
LIB_VERSION=6.1.6
ARCHITECTURE=i386
ROOT_DIR=usr/local

echo "VERSION is $VERSION"
echo "ARCHITECTURE is $ARCHITECTURE"
echo "Generating debian packaging into $DIST_DIR"

# copy source to local directories
cd $DIST_DIR


# create a source tarball for ACE and MADARA source
echo "Creating madara-$VERSION directory"
mkdir madara-$VERSION
cd madara-$VERSION

# we'll use the dpkg-deb script to read our debian directory, and
# dpkg-deb requires a DEBIAN folder and not the debian that dh does

mkdir DEBIAN

# create needed directories 
echo "Creating directories for source code and libraries"
mkdir -p $ROOT_DIR/include
mkdir -p $ROOT_DIR/lib
mkdir -p $ROOT_DIR/doc/madara

# copy the MADARA and ACE source includes into the appropriate directory
echo "Copying MADARA and ACE source trees for developers"
cp -r $MADARA_ROOT/include/madara $ROOT_DIR/include/
cp -r $ACE_ROOT/ace $ROOT_DIR/include/

# remove any shared objects, symbolic links and svn from the include directory
echo "Removing all symbolic links from source tree"
find $ROOT_DIR/include -type l -exec rm -f {} \;
echo "Removing all shared objects from source tree"
find $ROOT_DIR/include -name "*.so*" -type f -exec rm -f {} \;
echo "Removing all SVN directories from source tree"
find $ROOT_DIR/include -name .svn -type d -exec rm -rf {} \;

# update changelog with the last 10 entries from the repository
echo "Saving last 10 svn changelog entries into debian changelog"
svn log http://madara.googlecode.com/svn/trunk/ -l 10 > $ROOT_DIR/doc/madara/changelog

# merge MADARA and ACE copyright and license information
cat $MADARA_ROOT/LICENSE.txt >> $ROOT_DIR/doc/madara/copyright
cat $ACE_ROOT/COPYING >> $ROOT_DIR/doc/madara/copyright
dos2unix $ROOT_DIR/doc/madara/copyright

cp $ACE_ROOT/ace/libACE.so.$ACE_VERSION $ROOT_DIR/lib
cp $MADARA_ROOT/libMADARA.so.$LIB_VERSION $ROOT_DIR/lib
ln -s $ROOT_DIR/lib/libACE.so.$ACE_VERSION $ROOT_DIR/lib/libACE.so
ln -s $ROOT_DIR/lib/libMADARA.so.$LIB_VERSION $ROOT_DIR/lib/libMADARA.so 



# modify the control file to be specific to MADARA
echo "Package: madara" >> DEBIAN/control
echo "Priority: extra" >> DEBIAN/control
echo "Section: libs" >> DEBIAN/control
echo "Architecture: $ARCHITECTURE" >> DEBIAN/control
echo "Maintainer: James Edmondson <jedmondson@gmail.com>" >> DEBIAN/control 
echo "Version: $VERSION" >> DEBIAN/control
echo "Depends: debhelper (>= 8.0.0), libdl (>=2), librt (>=1), libstdc++ (>=6), libgcc (>=1), libpthread (>=0), libc (>=6), ld-linux (>=2)" >> DEBIAN/control
echo "Homepage: http:\/\/madara.googlecode.com" >> DEBIAN/control
echo "Description: Libraries for the MADARA middleware, version $VERSION" >> DEBIAN/control 



# create the debian package
cd ..
dpkg-deb -b madara-$VERSION

