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

#include <cstdint>
#include <cmath>

#include <string>
#include <vector>
#include <algorithm>

//system
//for make dir
#ifdef _WIN32
#include "Windows.h"
#else
#include "sys/types.h"
#include "sys/stat.h"
#endif

//libbluray
#include "libbluray/bluray.h"

#include "bdgetchap.h"

string convert_chap_time(uint64_t input)
{
    char *temp_res = new char[22];

    uint64_t secs = input / 90000;
    uint64_t hour = secs / 3600;
    uint64_t min = (secs / 60) % 60;
    uint64_t sec = secs % 60;
    uint64_t ms = (input / 90) % 1000;

#ifdef _WIN32
    sprintf(temp_res, "%02llu:%02llu:%02llu.%03llu", hour, min, sec, ms);
#else
    sprintf(temp_res, "%02lu:%02lu:%02lu.%03lu", hour, min, sec, ms);
#endif
    string res(temp_res);
    return res;
} //end convert_chap_time

double get_fps(uint8_t input)
{
    switch (input)
    {
    case 1:
        return 23.976;
    case 2:
        return 24;
    case 3:
        return 25;
    case 4:
        return 29.97;
    case 6:
        return 50;
    case 7:
        return 59.94;
    default:
        throw "Unknown FPS!";
    } //end switch
} //end get_fps

void bdgetchap_main_process(string &input, string &output)
{
    uint32_t min_duration = 0;
    int filter_flags = TITLES_RELEVANT;

    BLURAY *bd = nullptr;

    bd = bd_open(input.c_str(), nullptr);
    if (!bd)
    {
        throw("Fail to open BD!");
    }

    uint32_t n = bd_get_titles(bd, static_cast<uint8_t>(filter_flags), min_duration);

    //main loop
    vector<string> file_exist;
    vector<string> file_current;
    vector<string>::iterator it;
    for (uint32_t index = 0; index < n; ++index)
    {
        BLURAY_TITLE_INFO *title = bd_get_title_info(bd, index, 0);
        if (!title)
        {
            bd_close(bd);
            throw("Fail to get titles!");
        }

        if (title->chapter_count <= 1)
        {
            delete title;
            continue;
        }

        const BLURAY_TITLE_CHAPTER *chapters = title->chapters;

        string current_id;
        uint64_t base_time = chapters[0].start;
        uint32_t counter = 0;
        for (uint32_t index2 = 0; index2 < title->chapter_count; ++index2)
        {
            uint32_t clip_ref = chapters[index2].clip_ref;
            current_id = title->clips[clip_ref].clip_id;

            bool exist(false);
            if (file_exist.empty() == false)
            {
                it = std::find(file_exist.begin(), file_exist.end(), current_id);

                if (it != file_exist.end())
                {
                    exist = true;
                }
            }
            
            if (exist == false)
            {
                if (file_current.empty() == false)
                {
                    it = std::find(file_current.begin(), file_current.end(), current_id);

                    if (it == file_current.end())
                    {
                        file_current.push_back(current_id);
                    }
                }
                else
                {
                    file_current.push_back(current_id);
                }
                
                FILE *chap;
                FILE *qpfile;
#ifdef _WIN32
                errno_t chap_err, qpfile_err;
                chap_err = fopen_s(&chap, (output + "/" + current_id + ".txt").c_str(), "a");
                qpfile_err = fopen_s(&qpfile, (output + "/" + current_id + ".qpfile").c_str(), "a");
                if (chap_err != 0 || qpfile_err != 0)
#else
                chap = fopen((output + "/" + current_id + ".txt").c_str(), "a");
                qpfile = fopen((output + "/" + current_id + ".qpfile").c_str(), "a");
                if (!(chap) || !(qpfile))
#endif
                {
                    int mkdir_res;
#ifdef _WIN32
                    mkdir_res = CreateDirectoryA(output.c_str(), nullptr);
                    if (mkdir_res != 0)
                    {
                        chap_err = fopen_s(&chap, (output + "/" + current_id + ".txt").c_str(), "a");
                        qpfile_err = fopen_s(&qpfile, (output + "/" + current_id + ".qpfile").c_str(), "a");
                        if (chap_err != 0 || qpfile_err != 0)
                        {
                            delete title;
                            bd_close(bd);
                            throw("Fail to open output file!");
                        }
                    }
#else
                    mkdir_res = mkdir(output.c_str(), 0755);
                    if (mkdir_res != -1)
                    {
                        chap = fopen((output + "/" + current_id + ".txt").c_str(), "a");
                        qpfile = fopen((output + "/" + current_id + ".qpfile").c_str(), "a");
                        if (!chap || !qpfile)
                        {
                            delete title;
                            bd_close(bd);
                            throw("Fail to open output file!");
                        }
                    }
#endif
                    else
                    {
                        delete title;
                        bd_close(bd);
                        throw("Fail to create output directory!");
                    }
                }
                
                fprintf(chap, "CHAPTER%02u=", counter);
                uint64_t res = chapters[index2].start - base_time;
                fprintf(chap, "%s\n", convert_chap_time(res).c_str());
                fprintf(chap, "CHAPTER%02uNAME=\n", counter);
                
                double secs = 0.0000;
                secs = static_cast<double>(res) / 90000;
                double fps = 0.0000;
                try
                {
                    fps = get_fps(title->clips[clip_ref].video_streams->rate);
                }
                catch (...)
                {
                    fclose(chap);
                    fclose(qpfile);
                    delete title;
                    bd_close(bd);

                    throw "Fail to get FPS!";
                }

                int key_frame = static_cast<int>(floor((secs * fps) + 0.5));
                fprintf(qpfile, "%d K\n", key_frame);

                fclose(chap);
                fclose(qpfile);
            }
            ++counter;
            
            if (index2 < (title->chapter_count - 1))
            {
                uint32_t next_clip_ref = chapters[index2 + 1].clip_ref;

                if (clip_ref != next_clip_ref)
                {
                    base_time = chapters[index2 + 1].start;
                    counter = 0;
                }
            }
        }
        
        delete title;
        
        if (file_current.empty() == false)
        {
            unsigned int i(0);
            for (i = 0; i < file_current.size(); ++i)
            {
                file_exist.push_back(file_current.at(i));
            }
            
            file_current.clear();
        }
    } //end main loop

    bd_close(bd);
}