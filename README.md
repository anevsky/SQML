# SQML
SQWADRIX - Simple Page Viewer and Searcher

![alt Semantic Search Engine](images/SQWADRIX%20-%201.png "Semantic Search Engine")
![alt Semantic Search Engine](images/SQWADRIX%20-%202.png "Semantic Search Engine")
![alt Semantic Search Engine](images/SQWADRIX%20-%203.png "Semantic Search Engine")

*************************************************************************
    SQWADRIX - Simple Page Viewer and Searcher
    Copyright (C) 2010 Alex Nevsky, http://www.alexnevsky.com

    WebSite: http://www.alexnevsky.com
    Email: anwer.man@gmail.com
    Facebook: http://www.facebook.com/alexey.nevsky

*************************************************************************

1) Introduction.
2) How to run?
3) Source code and Qt.
4) Search Engine.
5) Shortest path.
6) Acknowledgments.

*************************************************************************

1) Introduction.

*************************************************************************

This is my lab for the course "Internet Technology", performed in the second year of study at the Belarusian State University Informatics and Radioelectronics.

Task was to develop a markup language (here - SQML), its viewer (here - SQWADRIX), the search engine (here - searcher) and finding the shortest path between two pages.

SQML does not support nested tags and strict with the syntax.

SQML syntax is described on the index page.

Pages should be with the extension .sqml and be the next template in UTF-8:

********

<sqml>
<head>
<title>Girls</title>
<meta annotation>Links to pages with the girls.</meta annotation>
<meta key>girls, girl</meta key>
</head>
[[pages/pageAshley.sqml]]

[[pages/pageXana.sqml]]

[[pages/pageKatie.sqml]]

Links to pages with the girls.
</sqml>

********

A detailed description of the syntax is displayed on the home page of SQWADRIX.

*************************************************************************

2) How to run?

*************************************************************************

You need a Qt - cross-platform application and UI framework.

It is free and available here: http://qt.nokia.com/products/.

After installing the Qt you can launch a SQWADRIX (bin/SQWADRIX) and everything will work. Just be sure that the same root folder contains a folder "pages" with our .sqml pages.

*************************************************************************

3) Source code and Qt.

*************************************************************************

Source code of the Qt project are located in "src".

!!!ATTENTION!!! When you compile your project, you must choose "Debug" mode!

I do not know why, but after compiling in "Release" mode, page's code (tags) are not processed. If you solve this issue, please let me know.

Everything definitely works on Mach OS X 10.6.5 and Qt 4.7 in "Debug" mode.

(You must run the application from the folder with app, not from Qt)

*************************************************************************

4) Search Engine.

*************************************************************************

Search engine is simple.

Code of each page contains the keywords that describe the page.

If the keywords from the search query matches the keyword page, the page gets points.

If the keywords from the search query matches with the title page, a page gets more points.

After which the results are ranked and displayed.

The accuracy of search depends on the page title and keywords describing it.

Try searching "girls", "porsche", "internet", "bill", "steve".

*************************************************************************

5) Shortest path.

*************************************************************************

To find the shortest path between two pages using graph.

At the core of a graph is Dijkstra's algorithm

Links - this is the arc of the graph.

There is a link - there is an arc.

Looking for the shortest path, count the number of arcs, show the way and the distance.

*************************************************************************

6) Acknowledgments.

*************************************************************************

Thank you for your interest!

*************************************************************************

From Alex Nevsky with Love, http://www.alexnevsky.com
