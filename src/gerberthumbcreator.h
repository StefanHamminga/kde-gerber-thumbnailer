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

#ifndef _GERBER_THUMB_CREATOR_H_
#define _GERBER_THUMB_CREATOR_H_

#include <QObject>
#include <kio/thumbcreator.h>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(LOG_GERBER_THUMB_CREATOR)

class GerberThumbCreator : public QObject, public ThumbCreator
{
    Q_OBJECT
public:
    GerberThumbCreator();
    virtual ~GerberThumbCreator();
    virtual bool create(const QString &path, int w, int h, QImage &thumb);
    virtual Flags flags() const;
};

#endif // _GERBER_THUMB_CREATOR_H_
