/*

    SQWADRIX - Simple Page Viewer and Searcher
    Copyright (C) 2010 Alex Nevsky, http://www.alexnevsky.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    WebSite: http://www.alexnevsky.com
    Email: anwer.man@gmail.com
    Facebook: http://www.facebook.com/alexey.nevsky

*/

/*

    Class:                 Viewer (viewer.h)
    $ver.1.0

    Author:                Alex Nevsky
    Created:               2010/05/25
    Email:                 anwer.man@gmail.com
    WebSite:               http://alexnevsky.com

*/

#ifndef VIEWER_H
#define VIEWER_H

#include <QtGui>

class Viewer : public QTextBrowser
{
    Q_OBJECT
    
public slots:
    void load(const QUrl &url);
    void pageLoaded();

public:
    Viewer();

    QFont *m_font;

    QTextCharFormat *m_textFormat;

    QUrl *m_url;

    QString pageCode;
    QString lastPage;
    QString curPage;

    QString pageTitle;

    void processCode();

    bool readFile(QString);

    QStringList findAllLinks(QString &pageCode);
    QString readPageCode(QString &pageName);

    void bon();
    void boff();
    void ion();
    void ioff();
    void uon();
    void uoff();
    void son();
    void soff();
    void coloron(QString);
    void coloroff();
    void sizeon(int);
    void sizeoff();
    void lefton();
    void leftoff();
    void righton();
    void rightoff();
    void centeron();
    void centeroff();
    void widthon();
    void widthoff();
    void nl();
    void ind();

    bool isMayBeTag(QChar &);
    bool isEnd(QString::iterator &);

    bool isLink(QString::iterator &);
    bool isImage(QString::iterator &);

    bool isBold(QString::iterator &);
    bool isItalic(QString::iterator &);
    bool isUnderline(QString::iterator &);
    bool isStrikeOut(QString::iterator &);

    bool isNl(QString::iterator &);
    bool isInd(QString::iterator &);

    bool isLeft(QString::iterator &);
    bool isRight(QString::iterator &);
    bool isCenter(QString::iterator &);
    bool isWidth(QString::iterator &);

    bool isColor(QString::iterator &);
    bool isSize(QString::iterator &);

    bool isSqml(QString::iterator &);
    bool isHead(QString::iterator &);
    bool isTitle(QString::iterator &);
    bool isMetaKey(QString::iterator &);
    bool isMetaAnnotation(QString::iterator &);

    bool isM(QString::iterator &);
};

#endif // VIEWER_H
