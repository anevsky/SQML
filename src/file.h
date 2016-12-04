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

    Class:                 File (file.h)
    $ver.1.0

    Author:                Alex Nevsky
    Created:               2009/10/20
    Email:                 anwer.man@gmail.com
    WebSite:               http://alexnevsky.com

*/

#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <iostream>
#include <string>

class File {
public:
    typedef std::string t_FileName;
    typedef std::string t_FileData;
    typedef bool t_SourceFileAvailable;

private:	
    t_FileName m_FileName;
    t_FileData m_FileData;
    t_SourceFileAvailable m_SourceFileAvailable;
public:	
    File() : m_FileName(), m_FileData(), m_SourceFileAvailable(false) {}
    File(const t_FileName& input_FileName) { Read(input_FileName); };

    bool Read(const t_FileName& input_FileName);
    bool Save(const t_FileData& input_DataToSave, t_FileName input_FileName);

    const bool GetData(std::string& output_Data) const;
    std::ostream& GetData(std::ostream& output_Stream) const;
    t_SourceFileAvailable IsSourceFileAvailable(void);
};

#endif
