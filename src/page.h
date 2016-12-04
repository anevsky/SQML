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

    Class:                 Page (page.h)
    $ver.1.0

    Author:                Alex Nevsky
    Created:               2010/05/25
    Email:                 anwer.man@gmail.com
    WebSite:               http://alexnevsky.com

*/

#ifndef PAGE_H
#define PAGE_H

#include <QtGui>

class Page
{
private:
    QString m_Name;
    QString m_Code;
    QString m_Annotation;
    QStringList m_KeyWords;
    QString m_Title;

    friend bool operator<(const Page&, const Page&);

public:
    Page();
    Page(QString input_Name);

    void SetName(QString input_Name);
    void SetCode(QString input_Code);
    void SetAnnotation(QString input_Annotation);
    void SetKeyWords(QString input_KeyWords);
    void SetTitle(QString input_Title);

    void ReadCode();
    void ReadAnnotation();
    void ReadKeyWords();
    void ReadTitle();

    QString GetName();
    QString GetCode();
    QString GetAnnotation();
    QStringList GetKeyWords();
    QString GetTitle();
};

inline
bool operator<(const Page &lhs, const Page &rhs) {
        if (lhs.m_Name < rhs.m_Name) {
                return true;
        } else {
                return false;
        }
}

#endif // PAGE_H
