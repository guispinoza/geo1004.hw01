// By Eleni Theodoridou & Guilherme Spinoza Andreo
#pragma once

#include "Point.h"

struct Point;
struct Dart;
struct Vertex;
struct Edge;
struct Face;
struct Volume;


struct Dart {

    int dart_id;
    // declaration to store dart's IDs later on

    // Identifiers for cells:
    int v;
    int e;
    int f;
    // involutions: 4 for a 3D map
    int a0;
    int a1;
    int a2;
    int a3;
};

struct Vertex {
    // the coordinates of this vertex:

    Point point;

    // constructor without arguments
    Vertex() : point(Point())
    {}

    // constructor with x,y,z arguments to immediately initialise the point member on this Vertex.
    Vertex(const double &x, const double &y, const double &z) : point(Point(x,y,z)){}


    //function to check for duplicates
    std::string coords_concat(const Point) {

      //converting the x,y,z into strings
      std::string str_x = std::to_string(point.x);
      std::string str_y = std::to_string(point.y);
      std::string str_z = std::to_string(point.z);

      //concatenate the strings into 1, which will be the unique key
      std::string s_xyz = str_x + str_y + str_z;

      return s_xyz; // return the unique xyz string
    }

    //bool operator needed because we created an unordered map that has as a key the struct Vertex
    bool operator==(const Vertex &other) const {
      return (point.x==other.point.x
              && point.y ==other.point.y
              && point.z == other.point.z);
    }
};

//hash function needed because of the use of the Vertex as key in an unordered map, specialize the std::hash template for Vertex
namespace std {
    template <>
    struct hash<Vertex>
    {
        std::size_t operator()(const Vertex& k) const
        {
//            using std::size_t;
//            using std::hash;
//            using std::string;

          return ((hash<double>()(k.point.x)
                   ^ (hash<double>()(k.point.y) << 1)) >> 1)
                 ^ (hash<double>()(k.point.z) << 1);
        }
    };
}

struct Edge {
    int id;
    int start;
    int end;


    // function to compute the barycenter for this Edge (needed for triangulation output):
    Vertex edge_baryc(std::vector<Vertex>&vertices, int &start, int &end){

      Vertex baryc;

      baryc.point=(vertices[start].point +vertices[end].point)/2;
      Vertex bar=baryc;

      return bar;
    }

    // function that creates a string out of the starting and ending point of an edge, to be used as a key in a map
    std:: string concat(const int &start, const int &end) {

      // covert seperately into string
      std::string v1 = std::to_string(start);
      std::string v2 = std::to_string(end);

      //concatenate the 2 strings
      std::string e = v1 + v2;

      return e;
    }

    //function that creates the inverse key (end, start) of an edge. to be used to avoid duplicate edges
    std:: string inv_concat(const int &start, const int &end) {

      // covert seperately into string
      std::string v1 = std::to_string(start);
      std::string v2 = std::to_string(end);

      //concatenate the 2 strings into the inverse
      std::string e_inv=v2 + v1;

      return e_inv;
    }

};

struct Face {
    int a,b,c,d;
    // a dart incident to this Face:

    //creates a unique string of the 4 vertices that create a face. to be used as key in a map
    std:: string face_concat(const int &a, const int &b, const int&c, const int &d) {

      std::string va = std::to_string(a);
      std::string vb = std::to_string(b);
      std::string vc = std::to_string(c);
      std::string vd = std::to_string(d);

      std::string f_abcd = va + vb +vc +vd;

      return f_abcd;
    }

    // function to compute the barycenter for this Face (needed for triangulation output):
    Vertex face_baryc(std::vector<Vertex>&vertices, int &a, int &b, int &c, int &d){

      Vertex baryc;

      baryc.point=(vertices[a].point +vertices[b].point +vertices[c].point +vertices[d].point)/4;
      Vertex bar=baryc;

      return bar;
    }
};

struct Volume {
    int a,b;
};

//new structure that is used for the triangulation. we define a face/dart that is created by 3 vertices
struct Tri_faces {
    int a, b, c;

    //bool operator needed because we created an unordered map that has as a key the struct Tri_faces
    bool operator==(const Tri_faces &other) const {
      return (a == other.a
              && b == other.b
              && c == other.c);
    }
};

//hash function needed because of the use of the Vertex as key in an unordered map, specialize the std::hash template for Tri_faces
namespace std {
    template<>
    struct hash< Tri_faces> {
        std::size_t operator()(const  Tri_faces &e) const {
          using std::size_t;
          using std::hash;
          using std::string;

          return ((hash<int>()(e.a)
                   ^ (hash<int>()(e.b) << 1)) >> 1)
                 ^ (hash<int>()(e.c) << 1);
        }
    };
}

