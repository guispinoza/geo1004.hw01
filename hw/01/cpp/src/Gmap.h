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
  // involutions:
  // Vertex to Vertex involution
  Dart* a0 = nullptr;
  // Vertex to Edge involution
  Dart* a1 = nullptr;
  // Edge dart to other Face dart involution
  Dart* a2 = nullptr;
  // A3?
  Dart* a3 = nullptr;

  // cells:
  // Vertex cells
  Vertex* v = nullptr;
  // Edge cells
  Edge* e = nullptr;
  // Face cells
  Face* f = nullptr;
  // Volume cells
  Volume* vol = nullptr;

  // constructor for darts
  Dart() {}
};

struct Vertex {
  // the coordinates of this vertex:
  Point point;
  // a dart incident to this Vertex:
  Dart* dart = nullptr;

  // constructor without arguments
  Vertex() : point(Point())
  {}

  // constructor with x,y,z arguments to immediately initialise the point member on this Vertex.
  Vertex(const double &x, const double &y, const double &z) : point(Point(x,y,z))
  {}

  //convert vertex coordinates into a unique string
  std::string getUniqueId () {
    return std::to_string(point.x) + std::to_string(point.y) + std::to_string(point.z);
  }

};

struct Edge {
  // a dart incident to this Edge:
  Dart* dart = nullptr;

  // empty constructor
  Edge() {};

  // function to compute the barycenter for this Edge (needed for triangulation output):
  Point barycenter() {
    return ( dart->v->point + dart->a0->v->point ) / 2;
  }
};

struct Face {
  Dart* dart = nullptr;

  // empty constructor
  Face() {};

  // function to compute the barycenter for this Face (needed for triangulation output):
  Point barycenter() {

    Dart* currentDart = dart->a0;
    int currentStep = 0;
    Point coordinateSum = Point();
    int visitedVertexCount = 0;

    while(currentDart != dart) {
      // Even steps
      if (currentStep % 2 == 0) {
        coordinateSum = coordinateSum + currentDart->v->point;
        visitedVertexCount += 1;
        currentDart = currentDart->a1;
      }
      // Odd steps
      else if (currentStep % 2 == 1) {
        currentDart = currentDart->a0;
      }
      currentStep += 1;
    }

    return coordinateSum / visitedVertexCount;
  }

};

struct Volume {
  // a dart incident to this Volume:
  Dart* dart = nullptr;

  // empty constructor
  Volume() {};

};