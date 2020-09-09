Build instructions
====================

These instructions are used to compile all Titcoin releases. The instructions are meant to be used on Debian 9 (Stretch). If you are using a different operating system, the necessary steps might be different. The recommended way to compile Titcoin is to set up Debian 9 (e.g. in a VM), make sure it has enough RAM and disk space and then follow these instructions.

```
# Install necessary tools
sudo apt-get install git build-essential libtool autotools-dev automake pkg-config bsdmainutils python3 curl wget

# Download + prepare Titcoin source code
git clone https://github.com/titcoin/titcoin
cd titcoin
./autogen.sh

# Build for Linux x64
cd depends
make HOST=x86_64-pc-linux-gnu
cd ..
CONFIG_SITE=$PWD/depends/x86_64-pc-linux-gnu/share/config.site ./configure --prefix=/
make clean
make
# For full distribution package (including man pages etc.), use:
#   mkdir linux-release
#   make install DESTDIR=$PWD/linux-release
# We are just packaging the binaries here:
mkdir linux-release
mv src/qt/titcoin-qt linux-release
mv src/titcoind linux-release
mv src/titcoin-cli linux-release
cd linux-release
tar -czvf ../titcoin-release.tar.gz *
cd ..
rm -r linux-release

# Build for Windows x64
sudo apt-get install g++-mingw-w64-x86-64 nsis
sudo update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix
cd depends
make HOST=x86_64-w64-mingw32
cd ..
CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure --prefix=/
make clean
make
make deploy
# if it says "error: could not build titcoin-0.16.3-win64-setup.exe", use:
makensis share/setup.nsi

# Build for Mac OS X 11
sudo apt-get install librsvg2-bin libtiff-tools cmake imagemagick libcap-dev libz-dev libbz2-dev python-setuptools libtinfo5
cd depends
mkdir -p SDKs sdk-sources
wget -N -P sdk-sources https://bitcoincore.org/depends-sources/sdks/MacOSX10.11.sdk.tar.gz
tar -C SDKs -xf sdk-sources/MacOSX10.11.sdk.tar.gz
make HOST=x86_64-apple-darwin11
cd ..
CONFIG_SITE=$PWD/depends/x86_64-apple-darwin11/share/config.site ./configure --prefix=/
make clean
make
make deploy
```
