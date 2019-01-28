/*
Copyright (C) 2019 fdar0536
This file is part of BDGetChapter.
BDGetChapter is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
BDGetChapter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with BDGetChapter.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifndef UTILS_H
#define UTILS_H

//qt
#include "QString"
#include "QListWidget"

bool find_bdmv_list(QListWidget*, QString&);

/*for log*/
QString set_date();

#endif // UTILS_H
