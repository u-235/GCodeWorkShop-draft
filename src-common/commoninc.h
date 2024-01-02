/***************************************************************************
 *   Copyright (C) 2006-2018 by Artur Kozioł                               *
 *   artkoz78@gmail.com                                                    *
 *                                                                         *
 *   This file is part of EdytorNC.                                        *
 *                                                                         *
 *   EdytorNC is free software; you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef COMMONINC_H
#define COMMONINC_H

// Uses in serialtransmissiondialog.cpp and mdichild.cpp
// TODO: move to CNC-specific class.
#define FILENAME_SINU840     "%_N_[a-zA-Z0-9_]{1,31}_(MPF|SPF|TEA|COM|PLC|DEF|INI)"
#define FILENAME_OSP         "\\$[A-Z]{1,1}[A-Z0-9_-]{1,}\\.(MIN|SSB|SDF|TOP|LIB|SUB|MSB)[%]{0,1}"
#define FILENAME_FANUC       "(?:\\n|\\r)(O|:)[0-9]{1,}"
#define FILENAME_SINU        "%\\b(MPF|SPF|TEA)[\\s]{0,3}[0-9]{1,4}\\b"
#define FILENAME_HEID1       "%[a-zA-Z0-9_]{1,30}(\\s)"
#define FILENAME_HEID2       "(BEGIN)(\\sPGM\\s)[a-zA-Z0-9_-+*]{1,}(\\sMM|\\sINCH)"
#define FILENAME_PHIL        "%PM[\\s]{1,}N[0-9]{4,4}"
#define FILENAME_FADAL       "(N1)(O|:)[0-9]{1,}"

#endif // COMMONINC_H
