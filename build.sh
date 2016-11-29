# Author : Gregory DEPUILLE
# Description : Build et package de l'application

CURRENT_DIR=$(pwd)

if [ -d "$CURRENT_DIR/build" ] ; then
  echo "Nettoyage build ..."
  rm -Rvf $CURRENT_DIR/build
fi
mkdir -p $CURRENT_DIR/build

if [ -d "$CURRENT_DIR/installer/packages/org.arig/data" ] ; then
  echo "Nettoyage installer data ..."
  rm -Rvf $CURRENT_DIR/installer/packages/org.arig/data
fi
mkdir -p $CURRENT_DIR/installer/packages/org.arig/data

echo "Build application ..."
cd $CURRENT_DIR/build
if [ -z $DEBIAN_FRONTEND ] ; then
  qmake-qt5 .. -r -spec linux-g++
else
  qmake .. -r -spec linux-g++
fi
make

echo "Copie binaire pour package ..."
cp -v RPLidarVisualizer $CURRENT_DIR/installer/packages/org.arig/data

if [ -z $DEBIAN_FRONTEND ] ; then
  cd $CURRENT_DIR
  if [ ! -d "$CURRENT_DIR/installer-framework" ] ; then
    git clone http://code.qt.io/cgit/installer-framework/installer-framework.git
  fi

  echo "Compilation outils de packaging ..."
  cd $CURRENT_DIR/installer-framework
  qmake-qt5
  make

  echo "Creation de l'installateur ..."
  cd $CURRENT_DIR/installer
  ../installer-framework/bin/binarycreator -c config/config.xml -p packages RPLidarVisualizerInstaller
  mv -v RPLidarVisualizerInstaller $CURRENT_DIR/build/
fi
