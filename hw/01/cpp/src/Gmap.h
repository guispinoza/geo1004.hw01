#pragma once

#include "Point.h"

struct Point;
struct Dart;
struct Vertex;
struct Edge;
struct Face;
struct Volume;

/*
Below you find the basic elements that you need to build the generalised map.
The main thing you need to fill out are the links between the elements:
  * the involutions and cells on the Dart
  * the darts on the cells

One way to do this is by using pointers. eg. define a member on the dart struct like

  Struct Dart {
    // involutions:
    Dart* a0 = nullptr;
    // ...

    // cells:
    // ...
  
  };

Then you could create and link Darts like:
  
  Dart* dart_a = new Dart();
  Dart* dart_b = new Dart();

  dart_a->a0 = dart_b;
*/

struct Dart {

    int dart_id;
    // cells:
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


    //int coords; //to create the unique key for the unordered map by concatenating the xyz

    // constructor without arguments
    Vertex() : point(Point())
    {}

    // constructor with x,y,z arguments to immediately initialise the point member on this Vertex.
    Vertex(const double &x, const double &y, const double &z) : point(Point(x,y,z)){}

    // a dart incident to this Vertex:
    int dart_v;

    //function to check for duplicates by converting the coords unique key
    std::string coords_concat(const Point) {
      std::string str_x = std::to_string(point.x);
      std::string str_y = std::to_string(point.y);
      std::string str_z = std::to_string(point.z);

      std:: string s_xyz = str_x + str_y + str_z;

      return s_xyz;
    }

    int retrieve (const int x){

    }

    bool operator==(const Vertex &other) const {
      return (point.x==other.point.x
              && point.y ==other.point.y
              && point.z == other.point.z);
    }
};

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


//    bool operator==(const Edge &other) const {
//        return (id == other.id
//                && start == other.start
//                && end == other.end);
//    }


//namespace std {
//    template<>
//    struct hash<Edge> {
//        std::size_t operator()(const Edge &e) const {
//            using std::size_t;
//            using std::hash;
//            using std::string;
//
//            return ((hash<int>()(e.id)
//                     ^ (hash<int>()(e.start) << 1)) >> 1)
//                   ^ (hash<int>()(e.end) << 1);
//        }
//    }
//}


    // a dart incident to this Edge:
    //Dart *dart = nullptr;

    // function to compute the barycenter for this Edge (needed for triangulation output):


    // Point barycenter() {}

    int concat(const int &start, const int &end) {

      // covert seperately into string
      std::string v1 = std::to_string(start);
      std::string v2 = std::to_string(end);

      //concatenate the 2 strings
      std::string v1_v2 = v1 + v2;

      // convert the string to int
      int e = stoi(v1_v2);

      return e;
    }

    int inv_concat(const int &start, const int &end) {

      // covert seperately into string
      std::string v1 = std::to_string(start);
      std::string v2 = std::to_string(end);

      //concatenate the 2 strings into the inverse
      std::string v2_v1 = v2 + v1;

      // convert the string to int
      int e_inv = stoi(v2_v1);

      return e_inv;
    }

};

struct Face {
    int a,b,c,d;
    // a dart incident to this Face:

    int face_concat(const int &a, const int &b, const int&c, const int &d) {
      std::string va = std::to_string(a);
      std::string vb = std::to_string(b);
      std::string vc = std::to_string(c);
      std::string vd = std::to_string(d);

      std::string f_abcd = va + vb +vc +vd;

      int abcd=stoi(f_abcd);

      return abcd;
    }

    // function to compute the barycenter for this Face (needed for triangulation output):
    Point face_baryc(std::vector<Vertex>&vertices, int &a, int &b, int &c, int &d){

      Vertex baryc;

      baryc.point=(vertices[a].point +vertices[b].point +vertices[c].point +vertices[d].point)/4;
      Point bar=baryc.point;

      return bar;
    }


    // Point barycenter() {}





    //xb = (x1 + x2) / 2
    //yb = (y1 + y2) / 2

};

struct Volume {
    // a dart incident to this Volume:
    // ...

};