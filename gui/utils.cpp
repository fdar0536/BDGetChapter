/*
Copyright (C) 2019 fdar0536
This file is part of BDGetChapter.
BDGetChapter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
BDGetChapter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with BDGetChapter.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "QString"
#include "QListWidget"
#include "QDateTime"

#include "utils.h"

bool find_bdmv_list(QListWidget *input, QString &parten)
{
    int index(0);
    for (index = 0; index < input->count(); ++index)
    {
        if (input->item(index)->text() == parten)
        {
            return true;
        }
    }
    
    return false;
}

QString set_date()
{
    QDateTime time(QDateTime::currentDateTime());
    return (time.toString("yyyy/MM/dd AP hh:mm:ss:zzz") + " ");
} //end set_date