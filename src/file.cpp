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

    Class implementation:  File (file.cpp)
    $ver.1.0

    Author:                Alex Nevsky
    Created:               2009/10/20
    Email:                 anwer.man@gmail.com
    WebSite:               http://alexnevsky.com

*/

#include "file.h"

//--------------------------------------------------------------
//
//--------------------------------------------------------------
bool File::Read(const t_FileName& input_FileName) 
{
	
	m_SourceFileAvailable = false;
	
	m_FileName = input_FileName;
	
	std::ifstream in;
	in.open(m_FileName.c_str());
	if (in) {
		std::string s;
		size_t linesNumber(0);
		while(!in.eof()) {
			getline(in, s);
			++linesNumber;
			if (linesNumber != 1)
				m_FileData = m_FileData + "\n" + s;
			else m_FileData += s;
		}
		m_SourceFileAvailable = true;
	}	
	else {
		m_SourceFileAvailable = false;
		std::cerr << "Error: unable to open input file \""
		<< input_FileName << "\"!" << std::endl;
	}
	in.close();
	
	return m_SourceFileAvailable;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
bool File::Save(
  const t_FileData& input_DataToSave, 
  t_FileName input_FileName
) 
{
	bool success(false);
	
	std::ofstream out;
	out.open(input_FileName.c_str());
	if (out) {
		out << input_DataToSave;
		success = true;
	}	
	else {
		success = false;
		std::cerr << "Error: unable to save data to file \""
		<< input_FileName << "\"!" << std::endl;
	}
	out.close();
	
	return success;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const bool File::GetData(std::string& output_Data) const 
{
	
	bool output_Success(false);
	
	if (m_SourceFileAvailable) {
		output_Data = m_FileData;
		output_Success = true;
	}
	
	return output_Success;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& File::GetData(std::ostream& output_Stream) const 
{
	
	output_Stream << m_FileData;
	
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
File::t_SourceFileAvailable File::IsSourceFileAvailable(void) 
{
	return m_SourceFileAvailable;
}
