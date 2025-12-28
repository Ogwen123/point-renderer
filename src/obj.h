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

RawObjFace take_face_data(std::string face_string);
RawObjVertex take_vert_data(std::string vert_string);
RawObjData load(std::string rel_path);
ObjData parse(RawObjData raw_data);
ObjData normalise(ObjData data);
ObjData obj_from_path(std::string path);

#endif