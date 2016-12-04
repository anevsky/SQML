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

    Class:                 MainWindow (mainwindow.h)
    $ver.1.0

    Author:                Alex Nevsky
    Created:               2010/05/25
    Email:                 anwer.man@gmail.com
    WebSite:               http://alexnevsky.com

*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "viewer.h"
#include "page.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:

private slots:
    void newPage();
    void loadPage();    

    void openPage(QString str);

    void lastPageButtonClicked();
    void sqrugleItButtonClicked();
    void minPathButtonClicked();
    void mainPageButtonClicked();

    void about();

private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void createDockWindows();

    QVector<Page> m_Pages;

    QMap<QString, int> m_Dictionary;
    QMultiMap<int, Page> m_SearchImages;

    void Request(QString);

    void Indexing();

    QStringList pageLinksAsEdges(QString page);

    bool loadPageFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    QString strippedName(const QString &fullFileName);
    QString curFile;

    Viewer *browser;
    QWidget *topWidget;
    QLineEdit *searchLineEdit;

    QPushButton *lastPageButton;
    QPushButton *sqrugleItButton;
    QPushButton *minPathButton;
    QPushButton *mainPageButton;

    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *newPageAction;
    QAction *loadPageAction;
    QAction *aboutAction;
    QAction *quitAction;
};

#endif // MAINWINDOW_H
