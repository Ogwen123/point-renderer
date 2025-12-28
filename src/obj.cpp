#include "obj.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>

RawObjFace take_face_data(std::string face_string)
{
    std::string buf;

    RawObjFace face = {};
    bool take = true;

    for (char c : face_string)
    {
        while (true)
        { // skip the string at the start of the line that denotes the type
            if (c == ' ')
            {
                break;
            }
        }

        if (c == ' ')
        {
            take = true;
            face.emplace_back(buf);
            buf = "";
        }
        else if (c == '/') // only take the number before the first slash
        {
            take = false;
        }
        else
        {
            if (take)
                buf.push_back(c);
        }
    }

    return face;
}

RawObjVertex take_vert_data(std::string vert_string)
{
    std::string buf;

    RawObjVertex vert = {};

    for (char c : vert_string)
    {
        while (true)
        { // skip the string at the start of the line that denotes the type
            if (c == ' ')
            {
                break;
            }
        }

        if (c == ' ')
        {
            vert.emplace_back(buf);
        }
        else
        {
            buf.push_back(c);
        }
    }

    return vert;
}

RawObjData load(std::string rel_path)
{

    std::vector<RawObjFace> faces = {};
    std::vector<RawObjVertex> verts = {};

    std::ifstream file(rel_path);
    std::string str;
    while (std::getline(file, str))
    {
        // only need to take faces and vertices
        if (str.substr(0, 1) == "f")
        {
            faces.emplace_back(take_face_data(str));
        }
        else if (str.substr(0, 2) == "v ")
        {
            verts.emplace_back(take_vert_data(str));
        }
    }

    return std::tuple<std::vector<RawObjFace>, std::vector<RawObjVertex>>{faces, verts};
}

// safely convert the string values to doubles
ObjData parse(RawObjData raw_data) { return {}; }
// normalise the values to fit in the -1...1 coord range being used
ObjData normalise(ObjData data) { return {}; }

ObjData obj_from_path(std::string path)
{
    std::cout << path << std::endl;
    RawObjData raw = load(path);

    return {};
}