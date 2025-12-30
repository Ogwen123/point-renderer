#ifndef OBJ_H

#define OBJ_H
#include <vector>
#include <tuple>
#include <string>

typedef std::vector<std::string> RawObjFace;
typedef std::vector<std::string> RawObjVertex;
typedef std::tuple<std::vector<RawObjFace>, std::vector<RawObjVertex>> RawObjData;

typedef std::vector<double> ObjFace;
typedef std::vector<double> ObjVertex;
typedef std::tuple<ObjFace, ObjVertex> ObjData;

class ObjData
{
public:
    std::vector<ObjFace> faces;
    std::vector<ObjVertex> vertices;
    ObjData(std::string path);

    static RawObjFace take_face_data(std::string face_string);
    static RawObjVertex take_vert_data(std::string vert_string);

    // safely convert the string values to doubles
    void parse(RawObjData raw_data);
    // normalise the values to fit in the -1...1 coord range being used
    void normalise();

    static RawObjData load(std::string rel_path);
};

#endif