# Gerber Thumbnail Creator for KDE 5

KDE thumbnail-plugin that generates small images (thumbnails) for Gerber files, to be displayed, for example, on Konqueror and Dolphin file managers.

## Dependencies

The following libraries and development packages are needed:
KDE >=5.2.x
QT  >=5.6.x
libgerbv (git master with X2 patch for best result)

## Building & installing

First install libgerbv using your package manager.

### Fetch the source

```bash
cd my_build_dir
git clone https://github.com/StefanHamminga/kde-gerber-thumbnailer
```

### Build

```bash
cd kde-gerber-thumbnailer
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=`kf5-config --prefix` ..
make
```

### Install

```bash
sudo make install
```

### Troubleshooting

If you've built, installed, and enabled the plugin, but no previews show up you might need to see if the library is installed in the proper location. You could compare the install location to the pretty common JPEG thumbnailer:

```bash
find /{usr,lib} -name "jpegthumbnail.so" -or -name "gerberthumbcreator.so" 2>/dev/null
```

The install path is set in the very last section of CMakeLists.txt.

## License & author

The Gerber thumbnailer for KDE 5 is written by [Stefan Hamminga](stefan@prjct.net) ([prjct.net](https://prjct.net)). The author is in no way connected to the Gerber brands, companies, trademarks, standards, etc.

This work is released under the terms of the LGPL 3.0 (GNU Lesser General Public License). The complete text is included in the `LICENSE` file. For online reference, consult:

http://www.fsf.org/licenses/lgpl.html

http://www.gnu.org/licenses/lgpl-3.0.txt

## Repository

https://github.com/StefanHamminga/kde-gerber-thumbnailer
