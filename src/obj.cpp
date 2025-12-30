#include "obj.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <stdexcept>

ObjData::ObjData(std::string path)
{
    // std::cout << path << std::endl;
    RawObjData raw;

    try
    {
        // std::cout << "loading" << std::endl;
        raw = load(path);
    }
    catch (const std::invalid_argument &e)
    {
        throw e;
    }
    // std::cout << "loaded" << std::endl;
    std::vector<RawObjFace> faces = std::get<0>(raw);

    // for (auto face : faces)
    // {
    //     for (auto point : face)
    //     {
    //         std::cout << point << ", ";
    //     }
    //     std::cout << std::endl;
    // }

    parse(raw);

    normalise();
}

RawObjFace ObjData::take_face_data(std::string face_string)
{
    std::string buf;

    RawObjFace face = {};
    bool adding = false;
    bool take = true;

    for (char c : face_string)
    {

        if (adding)
        {
            if (c == ' ')
            {
                take = true;
                if (buf.length() > 0)
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
        else
        {
            // skip the string at the start of the line that denotes the type
            if (c == ' ')
            {
                adding = true;
            }
        }
    }

    return face;
}

RawObjVertex ObjData::take_vert_data(std::string vert_string)
{
    std::string buf;

    RawObjVertex vert = {};
    bool adding = false;

    for (char c : vert_string)
    {
        if (adding)
        {
            if (c == ' ')
            {
                if (buf.length() > 0)
                    vert.emplace_back(buf);
            }
            else
            {
                buf.push_back(c);
            }
        }
        else
        {
            // skip the string at the start of the line that denotes the type
            if (c == ' ')
            {
                adding = true;
            }
        }
    }

    return vert;
}

// safely convert the string values to doubles
void ObjData::parse(RawObjData raw_data) {}
// normalise the values to fit in the -1...1 coord range being used
void ObjData::normalise() {}

RawObjData ObjData::load(std::string rel_path)
{

    std::vector<RawObjFace> faces = {};
    std::vector<RawObjVertex> verts = {};

    std::ifstream file(rel_path);

    if (!file.is_open())
    {
        // std::cout << "Could not open file with path provided." << std::endl;
        throw std::invalid_argument("Could not open file with path provided.");
    }
    int l = 0;
    std::string str;
    while (std::getline(file, str))
    {
        // only need to take faces and vertices
        if (str.substr(0, 1) == "f")
        {
            // std::cout << " face";
            faces.emplace_back(ObjData::take_face_data(str));
        }
        else if (str.substr(0, 2) == "v ")
        {
            // std::cout << " vert";
            verts.emplace_back(ObjData::take_vert_data(str));
        }
        // std::cout << std::endl;
    }
    file.close();
    return std::tuple<std::vector<RawObjFace>, std::vector<RawObjVertex>>{faces, verts};
}
