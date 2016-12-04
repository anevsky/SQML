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

    Class implementation:  Page (page.cpp)
    $ver.1.0

    Author:                Alex Nevsky
    Created:               2010/05/25
    Email:                 anwer.man@gmail.com
    WebSite:               http://alexnevsky.com

*/

#include "page.h"

Page::Page() {
}

Page::Page(QString input_Name) {
    SetName(input_Name);
}

void Page::SetName(QString input_Name) {
    m_Name = input_Name;

    ReadCode();
    ReadAnnotation();
    ReadKeyWords();
    ReadTitle();
}

void Page::SetCode(QString input_Code) {
    m_Code = input_Code;
}

void Page::SetAnnotation(QString input_Annotation) {
    m_Annotation = input_Annotation;
}

void Page::SetKeyWords(QString input_KeyWords) {
    m_KeyWords = input_KeyWords.toLower().split(", ");
}

void Page::SetTitle(QString input_Title) {
    m_Title = input_Title;
}

QString Page::GetName() {
    return m_Name;
}

QString Page::GetCode() {
    return m_Code;
}

QString Page::GetAnnotation() {
    return m_Annotation;
}

QStringList Page::GetKeyWords() {
    return m_KeyWords;
}

QString Page::GetTitle() {
    return m_Title;
}

void Page::ReadCode() {
    QFile file(m_Name);
    file.open(QIODevice::ReadOnly);

    SetCode(file.readAll());

    file.close();
}

void Page::ReadAnnotation() {
    int i = m_Code.indexOf("<meta annotation>");
    if (i != -1) {
        int ii = m_Code.indexOf("</meta annotation>");
        ii -= i;
        SetAnnotation(m_Code.mid(i + 17, ii - 17));
    }
}

void Page::ReadKeyWords() {
    int i = m_Code.indexOf("<meta key>");
    if (i != -1) {
        int ii = m_Code.indexOf("</meta key>");
        ii -= i;
        SetKeyWords(m_Code.mid(i + 10, ii - 10));
    }
}

void Page::ReadTitle() {
    int i = m_Code.indexOf("<title>");
    if (i != -1) {
        int ii = m_Code.indexOf("</title>");
        ii -= i;
        SetTitle(m_Code.mid(i + 7, ii - 7));
    }
}
