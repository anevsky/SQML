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

    Class implementation:  MainWindow (mainwindow.cpp)
    $ver.1.0

    Author:                Alex Nevsky
    Created:               2010/05/25
    Email:                 anwer.man@gmail.com
    WebSite:               http://alexnevsky.com

*/

#include "mainwindow.h"
#include "graph.h"
#include "page.h"

MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("SQWADRIX"));

    browser = new Viewer();

    setCentralWidget(browser);

    QPoint windowStartPoint(0, 0);
    move(windowStartPoint);

    browser->setFocus();    

    curFile = "Page Name";    

    connect(lastPageButton, SIGNAL(clicked()), this, SLOT(lastPageButtonClicked()));
    connect(sqrugleItButton, SIGNAL(clicked()), this, SLOT(sqrugleItButtonClicked()));
    connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT(sqrugleItButtonClicked()));
    connect(minPathButton, SIGNAL(clicked()), this, SLOT(minPathButtonClicked()));
    connect(mainPageButton, SIGNAL(clicked()), this, SLOT(mainPageButtonClicked()));

    Indexing();

    resize(700, 400);
}

void MainWindow::Indexing() {
    m_Pages.clear();
    m_Dictionary.clear();
    m_SearchImages.clear();

    QStringList filters;
    filters += "*.sqml";

    QString strPath(QDir::currentPath() + "/pages");

    QDir dir(strPath);

    QStringList filesList = dir.entryList(filters, QDir::Files);

    for (QStringList::iterator iter = filesList.begin(); iter != filesList.end(); ++iter) {
        *iter = ("pages/" + *iter);
    }

    for (QStringList::iterator iter = filesList.begin(); iter != filesList.end(); ++iter) {
        this->m_Pages.push_back(*iter);
    }

    if (true) {
        Page page;
        for (QVector<Page>::iterator iter = m_Pages.begin(); iter != m_Pages.end(); ++iter) {
            page = *iter;
            QStringList strListKeyWords = page.GetKeyWords();

            QString keyWord;
            for (QStringList::iterator iter = strListKeyWords.begin(); iter != strListKeyWords.end(); ++iter) {
                keyWord = *iter;
                if (!m_Dictionary.contains(keyWord.toLower())) {
                    m_Dictionary.insert(keyWord.toLower(), m_Dictionary.size());
                }
            }
        }
    }

    if (true) {
        Page page;
        for (QVector<Page>::iterator iter = m_Pages.begin(); iter != m_Pages.end(); ++iter) {
            page = *iter;
            QStringList strListKeyWords = page.GetKeyWords();

            QString keyWord;
            for (QStringList::iterator iter = strListKeyWords.begin(); iter != strListKeyWords.end(); ++iter) {
                keyWord = *iter;
                if (m_Dictionary.contains(keyWord.toLower())) {
                    m_SearchImages.insert(m_Dictionary[keyWord.toLower()], page);
                }
            }
        }
    }
}

void MainWindow::Request(QString input_Str) {
    input_Str = input_Str.toLower();

    QStringList list = input_Str.split(" ");
    list += input_Str.split(", ");

    QMap<Page, int> mapPages;

    QVector<int> indexVec;

    //select the indices of words in the search query and stores the vector
    for (QStringList::iterator iter = list.begin(); iter != list.end(); ++iter) {
        if (m_Dictionary.contains(*iter)) {
            indexVec.push_back(m_Dictionary[*iter]);
        }
    }

    //indexVec here contains the indices of key search words

    QList<Page> pagesL;
    for (QVector<int>::iterator iter = indexVec.begin(); iter != indexVec.end(); ++iter) {
        //retrieves a list of pages which contain one of the keywords
        pagesL = m_SearchImages.values(*iter);
        QList<QString> pagesList;
        Page page;
        for (QList<Page>::iterator iter = pagesL.begin(); iter != pagesL.end(); ++iter) {
            page = *iter;
            pagesList.append(page.GetName());
        }
        //scan the page and add points to the map pages with scores
        for (QList<QString>::iterator iter = pagesList.begin(); iter != pagesList.end(); ++iter) {
            int i = mapPages.value(*iter);

            //check the title and add the scores
            Page page;
            for (QList<Page>::iterator iterPage = pagesL.begin(); iterPage != pagesL.end(); ++iterPage) {
                page = *iterPage;
                if (page.GetName() == *iter && page.GetTitle().toLower() == input_Str) {
                    if (list.contains(page.GetTitle().toLower())) {
                        i += 35;
                        break;
                    }
                }
            }

            mapPages.insert(*iter, (i + 10));
        }
    }

    QMap<int, Page> mapResult;

    if (true) {
        QMapIterator<Page, int> iter(mapPages);
        while (iter.hasNext()) {
            iter.next();
            mapResult.insertMulti(iter.value(), iter.key());
        }
    }

    QFile fileWrite("pages/searchResult.sqml");
    fileWrite.open(QIODevice::WriteOnly);

    QTextStream out(&fileWrite);

    QString strOut;
    //top of search results page
    strOut = tr("<sqml><head><title>SQWADRIX - SQRUGLE It!</title></head>");

    strOut += tr("{{pages/SQLogo.png}}");
    strOut += tr(" $$nl$$ $$nl$$");

    strOut += tr("%%{size:15}Search Query:%% $$nl$$");
    strOut += input_Str;
    strOut += tr(" $$nl$$ $$nl$$");    

    strOut += tr("%%{size:15}Search Results:%% $$nl$$");

    strOut += tr("- __");
    QString strToNum;
    strOut += strToNum.setNum(mapResult.size());
    strOut += tr(" Documents Found__.");
    strOut += tr(" $$nl$$ $$nl$$");

    if (true) {
        QMapIterator<int, Page> iter(mapResult);
        int i = 1;
        iter.toBack();
        while (iter.hasPrevious()) {
            iter.previous();
            Page page = iter.value();
            QString str;
            str.setNum(i);
            strOut += "#**";
            strOut += str;
            strOut += ". ";
            strOut += " **";
            strOut += " [[";
            strOut += page.GetName();
            strOut += "|";
            strOut += page.GetTitle();
            strOut += "]] $$nl$$";
            strOut += " **";
            strOut += "Annotation: ";
            strOut += " **";
            strOut += page.GetAnnotation();
            strOut += " $$nl$$";
            strOut += " **";
            strOut += "Key Words: ";
            strOut += " **";
            strOut += page.GetKeyWords().join(", ");
            strOut += " $$nl$$";
            strOut += " **";
            strOut += "Relevance: ";
            strOut += " **";
            int rel = iter.key();
            str.setNum(rel);
            strOut += str;
            strOut += " points.";
            strOut += " $$nl$$";
            strOut += " **";
            strOut += "Link: ";
            strOut += " **";
            strOut += " [[";
            strOut += page.GetName();
            strOut += "|";
            strOut += page.GetName();
            strOut += "]] $$nl$$";
            strOut += " $$nl$$";

            ++i;
        }

        if (mapResult.isEmpty()) {
            strOut += tr("Does not match any documents matching the query :-(");
            strOut += " $$nl$$";
            strOut += " $$nl$$";
            strOut += tr("%%{size:15}Recommendations:%% $$nl$$");
            strOut += tr("- Make sure all words are spelled correctly. $$nl$$");
            strOut += tr("- Try different keywords. $$nl$$");
            strOut += tr("- Try more general keywords. $$nl$$");
            strOut += " $$nl$$";
        }

        strOut += tr("@**Thank you for choosing SQWADRIX!** $$nl$$");
    }

    /*
    if (true) {
        strOut += "\n@**Inside information:** \n";
        QMap<int, QString> mapResult;

        if (true) {
            QMapIterator<QString, int> iter(m_Dictionary);
            while (iter.hasNext()) {
                iter.next();
                mapResult[iter.value()] = iter.key();
            }
        }

        QString strn;
        QMapIterator<int, QString> iterDi(mapResult);
        strOut += "__m_Dictionary__\n";
        while (iterDi.hasNext()) {
            iterDi.next();
            strOut += strn.setNum(iterDi.key());
            strOut += " @ ";
            strOut += iterDi.value();
            strOut += "\n";
        }

        QMapIterator<int, Page> iterSe(m_SearchImages);
        strOut += "\n__m_SearchImages__\n";
        while (iterSe.hasNext()) {
            iterSe.next();
            strOut += strn.setNum(iterSe.key());
            strOut += " @ ";
            Page page = iterSe.value();
            strOut += page.GetName();
            strOut += "\n";
        }
    }
    */

    strOut += tr("</sqml>");    

    out << strOut;

    fileWrite.close();

    QUrl url("pages/searchResult.sqml");
    browser->load(url);
}

void MainWindow::lastPageButtonClicked() {
    browser->load(browser->lastPage);
}

void MainWindow::mainPageButtonClicked() {
    QUrl url("pages/index.sqml");
    browser->load(url);
}

void MainWindow::sqrugleItButtonClicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //update index
    Indexing();

    //carry out search
    Request(searchLineEdit->text());

    searchLineEdit->clear();

    QApplication::restoreOverrideCursor();
}

void MainWindow::minPathButtonClicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //file, which listed two pages, between which the search for the minimal path
    QFile file("minPath.txt");
    if (file.open(QIODevice::ReadOnly)) {

        QString str;
        str = file.readAll();
        file.close();

        QStringList strList;
        strList = str.split("\n");

        QFile fileWrite("pages/minPath.sqml");
        fileWrite.open(QIODevice::WriteOnly);

        QTextStream out(&fileWrite);

        QString strOut;
        //top of page
        strOut = tr("<sqml><head><title>The shortest path between two pages</title><meta key=""></meta></head>");

        strOut += tr("{{pages/SQLogo.png}}");
        strOut += tr(" $$nl$$ $$nl$$");

        //show addresses of two pages
        strOut += tr("%%{size:15}Pages (edit file: minPath.txt)%% $$nl$$");
        strOut += str;

        //shortest path
        QStringList filters;
        filters += "*.sqml";

        QString strPath(QDir::currentPath() + "/pages");

        QDir dir(strPath);

        QStringList filesList = dir.entryList(filters, QDir::Files);

        //parse the page and form a list of edges
        strOut += tr("\n\n%%{size:15}Shortest path:%% $$nl$$");

        QStringList strListWithLinks;

        for (QStringList::iterator iter = filesList.begin(); iter != filesList.end(); ++iter) {
            strListWithLinks += pageLinksAsEdges("pages/" + *iter);
        }

        QFile fileLinksToWrite("links.txt");
        fileLinksToWrite.open(QIODevice::WriteOnly);

        QTextStream linksOut(&fileLinksToWrite);

        for (QStringList::iterator iter = strListWithLinks.begin(); iter != strListWithLinks.end(); ++iter) {
            linksOut << *iter;
            linksOut << "\n";
        }

        fileLinksToWrite.close();

        //graph
        Graph g;
        g.SetFromFile("links.txt");

        QString ver1;
        ver1 = strList[0];

        QString ver2;
        ver2 = strList[1];

        std::string stdString(g.GetShortestPath(ver1.toStdString(), ver2.toStdString()));

        QString shPath;
        shPath.append(stdString.c_str());

        double d;
        d = g.GetDistanceFromVertexToVertex(ver1.toStdString(), ver2.toStdString());

        //strOut += shPath;

        QStringList shPathList;
        shPathList = shPath.split("--");
        for (QStringList::iterator iter = shPathList.begin(); iter != shPathList.end(); ++iter) {
            strOut += *iter;
            strOut += "\n";
        }

        strOut += "\n";

        QString strD;
        strD.setNum(d);

        strOut += tr("%%{size:15}The distance between the pages:%% $$nl$$");
        strOut += strD;
        strOut += "\n";
        strOut += "\n";

        /*
        strOut += tr("%%{size:15}Links:%% $$nl$$");
        for (QStringList::iterator iter = strListWithLinks.begin(); iter != strListWithLinks.end(); ++iter) {
            strOut += *iter;
            strOut += "\n";
        }
        */

        //page end
        strOut += tr("</sqml>");

        out << strOut;

        fileWrite.close();

        QUrl url("pages/minPath.sqml");
        browser->load(url);
    }

    QApplication::restoreOverrideCursor();
}

QStringList MainWindow::pageLinksAsEdges(QString page) {
    QStringList strList;

    //read the code of the page
    QString pageOneCode = browser->readPageCode(page);

    //highlight links
    QStringList pageOneLinksList = browser->findAllLinks(pageOneCode);

    //create a list of graph edges
    for (QStringList::iterator iter = pageOneLinksList.begin(); iter != pageOneLinksList.end(); ++iter) {
        strList << page + "--" + *iter;
    }

    return strList;
}

void MainWindow::newPage()
{

}

void MainWindow::loadPage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                               tr("Open - SQWADRIX"), ".",
                               tr("SQWADRIX files (*.sqml)"));
    if (!fileName.isEmpty()) {
        loadPageFile(fileName);
        curFile = fileName;
        //pathPageLineEdit->setText(curFile);
    }
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About SQWADRIX"),
            tr("<h2>«SQWADRIX»</h2>"

               "SQWADRIX - Simple Page Viewer and Searcher.<br><br>"
               "by Alex Nevsky, <a href=\"http://alexnevsky.com\">http://www.alexnevsky.com</a>"));

   statusBar()->showMessage(tr(""));
}

bool MainWindow::loadPageFile(const QString &fileName)
{    
    if (!browser->readFile(fileName)) {
        statusBar()->showMessage(tr("Upload canceled"), 2000);
        return false;
    }    

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File uploaded"), 2000);
    return true;
}

void MainWindow::createActions()
{
    newPageAction = new QAction(tr("&New"), this);
    newPageAction->setIcon(QIcon(":/images/new.png"));
    newPageAction->setShortcut(QKeySequence::New);
    newPageAction->setStatusTip(tr("New page"));
    connect(newPageAction, SIGNAL(triggered()), this, SLOT(newPage()));

    loadPageAction = new QAction(tr("&Load..."), this);
    loadPageAction->setIcon(QIcon(":/images/open.png"));
    loadPageAction->setShortcut(QKeySequence::Open);
    loadPageAction->setStatusTip(tr("Load a page from the file"));
    connect(loadPageAction, SIGNAL(triggered()), this, SLOT(loadPage()));

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setIcon(QIcon(":/images/Nuke.png"));
    quitAction->setShortcuts(QKeySequence::Quit);
    quitAction->setStatusTip(tr("Quit the program"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setIcon(QIcon(":/images/Info2.png"));
    aboutAction->setStatusTip(tr("Show information about the program"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Page"));
    //fileMenu->addAction(newPageAction);
    fileMenu->addAction(loadPageAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Done"));
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Navigation"), this);

    dock->setAllowedAreas(Qt::TopDockWidgetArea);

    topWidget = new QWidget();    

    QHBoxLayout *topOneLayour = new QHBoxLayout;

    searchLineEdit = new QLineEdit(tr(""));

    mainPageButton = new QPushButton(tr("Go To Main Page"));
    topOneLayour->addWidget(mainPageButton);

    lastPageButton = new QPushButton(tr("Go To Last Page"));
    topOneLayour->addWidget(lastPageButton);    

    topOneLayour->addStretch();

    topOneLayour->addWidget(searchLineEdit);

    sqrugleItButton = new QPushButton(tr("SQRUGLE It!"));
    minPathButton = new QPushButton(tr("Shortest Path"));

    topOneLayour->addWidget(sqrugleItButton);
    topOneLayour->addWidget(minPathButton);

    QHBoxLayout *topWidgetLayour = new QHBoxLayout;
    topWidgetLayour->addLayout(topOneLayour);

    topWidget->setLayout(topWidgetLayour);

    dock->setWidget(topWidget);
    addDockWidget(Qt::TopDockWidgetArea, dock);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                   .arg(tr("SQWADRIX")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::openPage(QString) {

}
