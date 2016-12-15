/*
 * Copyright (C)
 * 	 2016 Stefan Hamminga <stefan@prjct.net>
 *
 * This file is part of Gerber Thumbnail Creator.
 * This KDE 5 thumbnailer is in no way associated with the Gerber company,
 * trademarks or standards.
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with OpenDocument Thumbnail Creator.  If not, see
 * <http://www.gnu.org/licenses/> for a copy of the LGPLv3 License.
 */

#include "gerberthumbcreator.h"

#include <algorithm>

#include <QImage>
#include <QTemporaryFile>

#include <gerbv.h>

Q_LOGGING_CATEGORY(LOG_GERBER_THUMB_CREATOR, "GerberThumbCreator")

extern "C"
{
    Q_DECL_EXPORT ThumbCreator *new_creator() {
        return new GerberThumbCreator();
    }
}

GerberThumbCreator::GerberThumbCreator() { }

GerberThumbCreator::~GerberThumbCreator() { }

float lightness(GdkColor& c) {
    return (c.red / 65536.0 + c.green / 65536.0 + c.blue / 65536.0) / 3.0;
}

void darken(GdkColor& c) {
    if (c.red > 0 && c.green > 0 && c.blue > 0) {
        int lowest = std::min(c.red, std::min(c.green, c.blue));
        int highest = std::max(c.red, std::max(c.green, c.blue));

        float mult = highest / ((float)(highest - lowest));

        c.red   -= lowest;
        c.green -= lowest;
        c.blue  -= lowest;

        c.red   *= mult;
        c.green *= mult;
        c.blue  *= mult;
    }
}

bool color_equal(GdkColor &a, GdkColor &b) {
    if (a.red != b.red || a.green != b.green || a.blue != b.blue) {
        return false;
    }
    return true;
}


bool GerberThumbCreator::create(const QString &path, int w, int h, QImage &img) {
    // QLoggingCategory::setFilterRules(QStringLiteral("GerberThumbCreator.debug = true"));
    qCDebug(LOG_GERBER_THUMB_CREATOR) << "Gerber thumbnail generation for: " << path;

    if (w < 16 || h < 16) {
        w = 512;
        h = 512;
    }

    QByteArray tempinputbuf = path.toLocal8Bit();
    char* inputpath = tempinputbuf.data();

    // Hack to let Qt reserve us an unused filename for our temporary PNG image.
    QTemporaryFile tempfile("GerberThumb-XXXXXXXXXXXXXXXX.png");

    // tempfile.setAutoRemove(false);
    tempfile.open();

    auto fn = tempfile.fileName();
    QByteArray tempinputbuf2 = fn.toLocal8Bit();
    char* filename = tempinputbuf2.data();

    tempfile.close();

    /* create a top level libgerbv structure */
    gerbv_project_t *mainProject = gerbv_create_project();

    gerbv_open_layer_from_filename (mainProject, inputpath);

    /* make sure we parsed the files */
    if (mainProject->file[0] == NULL) {
        qCWarning(LOG_GERBER_THUMB_CREATOR) << "Unable to read Gerber file: " << path;
        return false;
    }

    GdkColor black {0,     0,     0,     0};
    GdkColor white {0, 65535, 65535, 65535};

    /* Use a white thumb background for nice contrast */
    mainProject->background = white;

    mainProject->file[0]->alpha = 65535; // Set the layer opacity to full

    // mainProject->file[0]->color = black;

    /* Adjust the brightness down a bit to make the Gerber data stand out more. If you want just
       black and white thumbs, comment this block out and uncomment above line. */
    if (color_equal(mainProject->file[0]->color, white)) {
        mainProject->file[0]->color = black;
    } else {
        darken(mainProject->file[0]->color);
    }


    /* export a rendered PNG image of the project, using the autoscale version
       to automatically center the image */
    gerbv_render_info_t renderInfo = {1, 1, 0, 0, GERBV_RENDER_TYPE_CAIRO_HIGH_QUALITY, w, h, false};

    gerbv_render_zoom_to_fit_display (mainProject, &renderInfo);
    gerbv_export_png_file_from_project (mainProject, &renderInfo, filename);


    /* destroy the project, which will in turn destroy all child images */
    gerbv_destroy_project (mainProject);

    QImage timg;
    timg.load(filename);

    if (!timg.isNull()) {

        auto timg2 = timg.convertToFormat(QImage::Format_ARGB32);

        // New background color (white / transparent)
        auto trs = qRgba(255, 255, 255, 0);

        QColor c;

        int x, y, w = img.width(), q = img.height();

        for (y = 0; y < q; y++) {
            for (x = 0; x < w; x++) {
                c = img.pixelColor(x, y);
                if (c.red() == 255 && c.green() == 255 && c.blue() == 255) {
                    img.setPixelColor(x, y, trs);
                }
            }
        }

        img.swap(timg2);

        return true;
    }
    qCWarning(LOG_GERBER_THUMB_CREATOR) << "Unable to read temp png file: " << filename << ", for: " << path;
    return false;
}

ThumbCreator::Flags GerberThumbCreator::flags() const {
    // return (Flags)(DrawFrame | BlendIcon);
    return DrawFrame;
    // return None;
}
