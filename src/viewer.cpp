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

    Class implementation:  Viewer (viewer.cpp)
    $ver.1.0

    Author:                Alex Nevsky
    Created:               2010/05/25
    Email:                 anwer.man@gmail.com
    WebSite:               http://alexnevsky.com

*/

#include "viewer.h"

Viewer::Viewer()
{
    m_font = new QFont;

    m_textFormat = new QTextCharFormat;

    m_url = new QUrl;

    QUrl startUrl("pages/index.sqml");

    load(startUrl);

    connect(this, SIGNAL(anchorClicked(QUrl)), SLOT(load(QUrl)));
}

bool Viewer::readFile(QString fileName) {
    this->clear();

    this->setCurrentCharFormat(QTextCharFormat());

    this->setCurrentFont(*this->m_font);

    this->lastPage = this->curPage;

    this->curPage = fileName;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    pageCode = file.readAll();
    file.close();

    this->sizeoff();

    processCode();

    return true;
}

QString Viewer::readPageCode(QString &pageName) {
    QFile file(pageName);
    file.open(QIODevice::ReadOnly);

    QString pageCode = file.readAll();

    file.close();

    return pageCode;
}

void Viewer::load(const QUrl &url) {
    //QDir dir = QDir::root();
    //this->readFile(dir.absoluteFilePath(url.toString()));
    this->pageTitle.clear();
    this->readFile(url.toString());
    emit pageLoaded();
}

void Viewer::bon() {
    m_font->setBold(true);
    this->setCurrentFont(*m_font);
}

void Viewer::boff() {
    m_font->setBold(false);
    this->setCurrentFont(*m_font);
}

void Viewer::ion() {
    m_font->setItalic(true);
    this->setCurrentFont(*m_font);
}

void Viewer::ioff() {
    m_font->setItalic(false);
    this->setCurrentFont(*m_font);
}

void Viewer::uon() {
    m_font->setUnderline(true);
    this->setCurrentFont(*m_font);
}

void Viewer::uoff() {
    m_font->setUnderline(false);
    this->setCurrentFont(*m_font);
}

void Viewer::son() {
    m_font->setStrikeOut(true);
    this->setCurrentFont(*m_font);
}

void Viewer::soff() {
    m_font->setStrikeOut(false);
    this->setCurrentFont(*m_font);
}

void Viewer::coloron(QString str) {
    if (str == "red") {
        this->setTextColor(Qt::red);
    }
    if (str == "blue") {
        this->setTextColor(Qt::blue);
    }
    if (str == "green") {
        this->setTextColor(Qt::green);
    }
    if (str == "darkBlue") {
        this->setTextColor(Qt::darkBlue);
    }
    if (str == "darkRed") {
        this->setTextColor(Qt::darkRed);
    }
}

void Viewer::coloroff() {
    setTextColor(Qt::black);
}

void Viewer::sizeon(int fSize) {
    m_font->setPointSize(fSize);
    setCurrentFont(*m_font);

}

void Viewer::sizeoff() {
    m_font->setPointSize(12);
    setCurrentFont(*m_font);

}

void Viewer::lefton() {
   this->setAlignment(Qt::AlignLeft);
}

void Viewer::leftoff() {
    this->insertPlainText("\n");
    this->setAlignment(Qt::AlignLeft);
}

void Viewer::righton() {
    this->setAlignment(Qt::AlignRight);
}

void Viewer::rightoff() {
    this->insertPlainText("\n");
    this->setAlignment(Qt::AlignLeft);
}

void Viewer::centeron() {
    this->setAlignment(Qt::AlignCenter);
}

void Viewer::centeroff() {
    this->insertPlainText("\n");
    this->setAlignment(Qt::AlignLeft);
}

void Viewer::widthon() {
    this->setAlignment(Qt::AlignJustify);
}

void Viewer::widthoff() {
    this->insertPlainText("\n");
    this->setAlignment(Qt::AlignLeft);
}

void Viewer::nl() {

}

void Viewer::ind() {

}

void Viewer::pageLoaded() {

}

QStringList Viewer::findAllLinks(QString &pageCode) {
    QStringList linksList;
    QString link;
    for (QString::iterator iter = pageCode.begin(); iter != pageCode.end(); ++iter) {
        if (*iter == '[') {
            ++iter;
            if (*iter == '[') {
                ++iter;
                while (*iter != ']' && *iter != '|') {
                    link += *iter;
                    ++iter;
                }
            }
            if (!link.isEmpty()) {
                linksList << link;
                link.clear();
            }
        }
    }
    return linksList;
}

void Viewer::processCode() {
    QString workPageCode(pageCode);

    //removing whitespace including newline
    //workPageCode = workPageCode.simplified();

    if (1) {
        int i = workPageCode.indexOf("<title>");
        if (i != -1) {
            int ii = workPageCode.indexOf("</title>");
            ii -= i;
            this->pageTitle = workPageCode.mid(i + 7, ii - 7);
        }
    }

    if (1) {
        int i = workPageCode.indexOf("</head>\n");
        if (i != -1) {
            workPageCode.remove(0, i + 8);
        } else {
            int i = workPageCode.indexOf("</head>");
            if (i != -1) {
                workPageCode.remove(0, i + 7);
            }
        }
    }

    this->sizeon(22);
    this->centeron();
    this->coloron("darkBlue");
    this->insertPlainText(this->pageTitle + "\n");
    this->coloroff();
    this->sizeoff();
    this->centeroff();

    QString str;

    for (QString::iterator iter = workPageCode.begin(); iter != workPageCode.end(); ++iter) {
        if (isMayBeTag(*iter)) {
            if (!str.isNull()) {
                this->insertPlainText(str);
                str.clear();
            }
            if (!isEnd(iter)) {
                //check for a match all tags
                isBold(iter);
                isItalic(iter);
                isUnderline(iter);
                isStrikeOut(iter);

                isLink(iter);

                isNl(iter);
                isInd(iter);

                isImage(iter);

                isLeft(iter);
                isRight(iter);
                isCenter(iter);
                isWidth(iter);

                isSize(iter);
                isColor(iter);

                isM(iter);

                isSqml(iter);
                //isHead(iter);
                //isTitle(iter);
                //isMetaKey(iter);
                //isMetaAnnotation(iter);
            }
        }
        str += *iter;
    }    
    QTextCursor cursor(this->textCursor());
    cursor.movePosition(QTextCursor::Start);
    this->setTextCursor(cursor);
}

bool Viewer::isM(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '<') {
        ++iter;
        if (*iter == 'm') {
            ++iter;
            ++iter;
            ++it;
            ++it;
            ++it;
            while (*iter != '<') {
                this->insertPlainText(*it);
                ++iter;
                ++it;
            }
            ++it;
            ++it;
            ++it;
            ++it;
        }
    }
}

bool Viewer::isSqml(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '<') {
        ++iter;
        if (*iter == 's') {
            ++it;
            ++it;
            ++iter;
            while (*iter != '>') {
                ++iter;
                ++it;
            }
            ++iter;
            ++it;
        } else {
            if (*iter == '/') {
                ++iter;
                if (*iter == 's') {
                    while (*it != '>') {
                        ++it;
                    }
                    *it = ' ';
                  }
            }
        }
    }
}

bool Viewer::isHead(QString::iterator &it) {

}

bool Viewer::isTitle(QString::iterator &it) {

}

bool Viewer::isMetaKey(QString::iterator &it) {

}

bool Viewer::isMetaAnnotation(QString::iterator &it) {

}


bool Viewer::isImage(QString::iterator &it) {
    QString::iterator iter = it;    
    if (*iter == '{') {
        ++iter;
        if (*iter == '{') {
            ++it;
            ++it;
            ++iter;
            QString str;
            while (*iter != '}') {
                str += *iter;
                ++iter;
                ++it;
            }
            ++iter;
            ++it;
            ++iter;
            ++it;
            this->textCursor().insertImage(str);
            this->setFont(*this->m_font);
        }
    }    
}

bool Viewer::isLink(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '[') {
        ++iter;
        if (*iter == '[') {
            ++it;
            ++it;
            ++iter;
            QString str;
            while (*iter != ']' && *iter != '|') {
                str += *iter;
                ++iter;
                ++it;
            }
            QString linkName;
            if (*iter == '|') {
                ++iter;
                ++it;
                while (*iter != ']') {
                    linkName += *iter;
                    ++iter;
                    ++it;
                }
            }
            ++iter;
            ++it;
            ++iter;
            ++it;

            QTextCharFormat lf;
            lf.setAnchor(true);
            lf.setAnchorHref(str);
            this->setCurrentCharFormat(lf);

            this->uon();
            this->coloron("blue");

            if (linkName.isNull()) {
                this->insertPlainText(str);
            } else {
                this->insertPlainText(linkName);
            }            

            this->uoff();
            this->coloroff();

            this->setCurrentCharFormat(QTextCharFormat());

            this->setCurrentFont(*this->m_font);
        }
    }    
}

bool Viewer::isLeft(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '$') {
        ++iter;
        if (*iter == '$') {
            ++iter;
            ++iter;
            ++iter;
            if (*iter == 'l') {
                ++it;
                ++it;
                ++it;
                while (*iter != '}') {
                    ++iter;
                    ++it;
                }
                ++iter;
                ++it;
                this->lefton();
                while (*iter != '$') {
                    this->insertPlainText(*iter);
                    ++iter;
                    ++it;
                }
                ++it;
                ++it;
                ++it;
                this->leftoff();
            }
        }
    }
}

bool Viewer::isRight(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '$') {
        ++iter;
        if (*iter == '$') {
            ++iter;
            ++iter;
            ++iter;
            if (*iter == 'r') {
                ++it;
                ++it;
                ++it;
                while (*iter != '}') {
                    ++iter;
                    ++it;
                }
                ++iter;
                ++it;
                this->righton();
                while (*iter != '$') {
                    this->insertPlainText(*iter);
                    ++iter;
                    ++it;
                }
                ++it;
                ++it;
                ++it;
                this->rightoff();
            }
        }
    }
}

bool Viewer::isCenter(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '$') {
        ++iter;
        if (*iter == '$') {
            ++iter;
            ++iter;
            ++iter;
            if (*iter == 'c') {
                ++it;
                ++it;
                ++it;
                while (*iter != '}') {
                    ++iter;
                    ++it;
                }
                ++iter;
                ++it;
                this->centeron();
                while (*iter != '$') {
                    this->insertPlainText(*iter);
                    ++iter;
                    ++it;
                }
                ++it;
                ++it;
                ++it;
                this->centeroff();
            }
        }
    }
}

bool Viewer::isWidth(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '$') {
        ++iter;
        if (*iter == '$') {
            ++iter;
            ++iter;
            ++iter;
            if (*iter == 'w') {
                ++it;
                ++it;
                ++it;
                while (*iter != '}') {
                    ++iter;
                    ++it;
                }
                ++iter;
                ++it;
                this->widthon();
                while (*iter != '$') {
                    this->insertPlainText(*iter);
                    ++iter;
                    ++it;
                }
                ++it;
                ++it;
                ++it;
                this->widthoff();
            }
        }
    }
}


bool Viewer::isColor(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '%') {
        ++iter;
        if (*iter == '%') {
            ++iter;
            ++iter;
            if (*iter == 'c') {
                ++it;
                ++it;
                ++it;
                while (*iter != ':') {
                    ++iter;
                    ++it;
                }
                ++iter;
                ++it;
                QString str;
                while (*iter != '}') {
                    str += *iter;
                    ++iter;
                    ++it;
                }
                ++iter;
                ++it;
                this->coloron(str);
                while (*iter != '%') {
                    this->insertPlainText(*iter);
                    ++iter;
                    ++it;
                }
                ++it;
                ++it;
                this->coloroff();
            }
        }
    }
}

bool Viewer::isSize(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '%') {
        ++iter;
        if (*iter == '%') {            
            ++iter;
            ++iter;
            if (*iter == 's') {
                ++it;
                ++it;
                ++it;
                while (*iter != ':') {
                    ++iter;
                    ++it;
                }
                ++iter;
                ++it;
                QString str;
                while (*iter != '}') {
                    str += *iter;
                    ++iter;
                    ++it;
                }
                ++iter;
                ++it;
                this->sizeon(str.toInt());
                while (*iter != '%') {
                    this->insertPlainText(*iter);
                    ++iter;
                    ++it;
                }
                ++it;
                ++it;
                this->sizeoff();
            }
        }
    }
}

bool Viewer::isBold(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '*') {
        ++iter;
        if (*iter == '*') {
            ++it;
            ++it;
            if (!this->m_font->bold()) {
                bon();
            } else {
                boff();
            }
        }
    }
}

bool Viewer::isItalic(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '/') {
        --iter;
        if (*iter != ':') {
            ++iter;
            ++iter;
            if (*iter == '/') {
                ++it;
                ++it;
                if (!this->m_font->italic()) {
                    ion();
                } else {
                    ioff();
                }
            }
        }
    }
}

bool Viewer::isUnderline(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '_') {
        ++iter;
        if (*iter == '_') {
            ++it;
            ++it;
            if (!this->m_font->underline()) {
                uon();
            } else {
                uoff();
            }
        }
    }
}

bool Viewer::isStrikeOut(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '!') {
        ++iter;
        if (*iter == '!') {
            ++it;
            ++it;
            ++iter;
            if (!this->m_font->strikeOut() && *iter != '!') {
                son();
                --iter;
            } else {
                soff();
                --iter;
            }
        }
    }
}


bool Viewer::isNl(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '$') {
        ++iter;
        if (*iter == '$') {            
            ++iter;
            if (*iter == 'n') {
                ++it;
                ++it;
                ++it;
                ++it;
                ++it;
                ++it;
                ++iter;
                this->append("");
            }
        }
    }
}

bool Viewer::isInd(QString::iterator &it) {
    QString::iterator iter = it;
    if (*iter == '$') {
        ++iter;
        if (*iter == '$') {            
            ++iter;
            if (*iter == 'i') {
                ++it;
                ++it;
                ++it;
                ++it;
                ++it;
                ++it;
                ++it;
                ++iter;
                ++iter;
                this->insertPlainText("   ");
            }
        }
    }
}

bool Viewer::isEnd(QString::iterator &it) {
    QString::iterator iter = it;
    ++iter;
    if (*iter == '//') {
        ++iter;
        if (*iter == 's') {
            while (*it != '>') {
                ++it;
            }
            *it = ' ';
            return true;
        } else {
            return false;
        }
    }
}

bool Viewer::isMayBeTag(QChar &c) {
    if (c == '*' || c == '/' || c == '_' || c == '!' ||
        c == '<' || c == '%' || c == '$' || c == '[' || c == '{') {
        return true;
    } else {
        return false;
    }
}
