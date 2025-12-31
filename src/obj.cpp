#include "obj.h"

parse_error::parse_error(char const *const message) noexcept : std::runtime_error(message)
{
}
const char *parse_error::what() const noexcept
{
    return std::runtime_error::what();
}

ObjData::ObjData()
{
}
ObjData::ObjData(std::string path)
{
    std::cout << path << std::endl;
    RawObjData raw;

    try
    {
        std::cout << "Loading ";
        raw = load(path);
    }
    catch (const std::invalid_argument &e)
    {
        throw e;
    }
    std::cout << "\033[32m[DONE]\033[39m" << std::endl;
    std::vector<RawObjFace> faces = std::get<0>(raw);

    // for (auto face : faces)
    // {
    //     for (auto point : face)
    //     {
    //         std::cout << point << ", ";
    //     }
    //     std::cout << std::endl;
    // }

    try
    {
        std::cout << "Parsing ";
        parse(raw);
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "\033[31m[FAIL]\033[39m" << std::endl;
        throw e;
    }
    std::cout << "\033[32m[DONE]\033[39m" << std::endl;

    try
    {
        std::cout << "Normalising Coordinates ";
        normalise();
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "\033[31m[FAIL]\033[39m" << std::endl;
        throw e;
    }
    std::cout << "\033[36m[WIP]\033[39m" << std::endl;
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

    // add data left in buf
    if (buf.length() > 0)
        face.emplace_back(buf);

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
                buf = "";
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

    if (vert.size() != 3)
    { // if already at 3 then the w value is in the buffer and can be ignored
        vert.emplace_back(buf);
    }

    return vert;
}

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

    // for (auto vert : verts)
    // {
    //     std::cout << "vert: ";
    //     for (auto p : vert)
    //     {
    //         std::cout << p << "   ";
    //     }
    //     std::cout << std::endl;
    // }

    // for (auto face : faces)
    // {
    //     std::cout << "face: ";
    //     for (auto p : face)
    //     {
    //         std::cout << p << "   ";
    //     }
    //     std::cout << std::endl;
    // }

    file.close();
    return std::tuple<std::vector<RawObjFace>, std::vector<RawObjVertex>>{faces, verts};
}

// safely convert the string values to doubles
void ObjData::parse(RawObjData raw_data)
{
    std::vector<RawObjFace> raw_faces = std::get<0>(raw_data);
    std::vector<RawObjVertex> raw_verts = std::get<1>(raw_data);

    std::vector<ObjFace> faces = {};
    std::vector<ObjVertex> verts = {};

    for (auto raw_vert : raw_verts)
    {
        ObjVertex vert = {};
        for (auto point : raw_vert)
        {
            try
            {
                vert.emplace_back(std::stod(point));
            }
            catch (std::invalid_argument &e)
            {
                throw parse_error(std::format("could not parse %s into a string", point).c_str());
            }
        }
    }

    for (auto raw_face : raw_faces)
    {
        ObjFace face = {};
        for (auto point : raw_face)
        {
            try
            {
                face.emplace_back(std::stod(point));
            }
            catch (std::invalid_argument &e)
            {
                throw parse_error(std::format("could not parse %s into a string", point).c_str());
            }
        }
    }

    this->faces = faces;
    this->vertices = verts;
}
// normalise the values to fit in the -1...1 coord range being used
void ObjData::normalise() {}
