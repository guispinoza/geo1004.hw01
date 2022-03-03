// some standard libraries that are helpfull for reading/writing text files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

#include "Gmap.h"



int main(int argc, const char * argv[]) {
  std::string file_in = "cube.obj";
  std::string file_out_obj = "C:/Users/etheo/Documents/Geomatics/Q3/GEO1004_3D Modelling of the built Environment/geo1004.2022/hw/01/data/torus_triangulated.obj";
  std::string file_out_csv_d = "C:/Users/etheo/Documents/Geomatics/Q3/GEO1004_3D Modelling of the built Environment/geo1004.2022/hw/01/data/output_darts.csv";
  std::string file_out_csv_0 = "C:/Users/etheo/Documents/Geomatics/Q3/GEO1004_3D Modelling of the built Environment/geo1004.2022/hw/01/data/torus_vertices.csv";
  std::string file_out_csv_1 = "C:/Users/etheo/Documents/Geomatics/Q3/GEO1004_3D Modelling of the built Environment/geo1004.2022/hw/01/data/output_edges.csv";
  std::string file_out_csv_2 = "C:/Users/etheo/Documents/Geomatics/Q3/GEO1004_3D Modelling of the built Environment/geo1004.2022/hw/01/data/torus_faces.csv";
  std::string file_out_csv_3 = "C:/Users/etheo/Documents/Geomatics/Q3/GEO1004_3D Modelling of the built Environment/geo1004.2022/hw/01/data/torus_volume.csv";

  // ## Read OBJ file ##

  // Example how to read vertices from an obj file and put it in a vector:

  std::ifstream stream_in;
  stream_in.open(file_in);
  std::vector<Vertex> vertices;
  std::vector<Face> init_faces;
  //bool wtf = stream_in.is_open();
  if (stream_in.is_open()) {
    std::string line;
    while (getline(stream_in, line)) {
      std::istringstream iss(line);
      std::string word;
      iss >> word;
      if (word == "v") {
        std::vector<float> coordinates;
        while (iss >> word) coordinates.push_back(std::stof(word)); // stof: converts string to float
        if (coordinates.size() == 3) vertices.emplace_back(coordinates[0], coordinates[1], coordinates[2]);
        else vertices.push_back(Vertex());
      }

      // ## read the faces ##
      if (word == "f") {
        std::vector<int> index;
        while (iss >> word) index.push_back(std::stoi(word) -
                                            1); //stoi converts str to int // we also substract 1 bc obj starts at 1 and C++ starts at 0 indexing
        if (index.size() == 4) init_faces.emplace_back(Face{index[0], index[1], index[2], index[3]});
        //else init_faces.push_back(Face());
      }

    }
  }

  int id_e = 1;
  int id_f = 1;
  int id_v = 1;
  int id_d = 1;

  std::vector<Edge> edges;
  std::vector<Dart> darts;
  std::vector<Face> faces;

  std::unordered_map<int, int> e_map;
  //std::unordered_map<Vertex,int>v_map;
  std::unordered_map<Vertex, int> v_map;
  std::unordered_map<int, int> f_map;

  int id_e_exist;

  for (Face f: init_faces) {
    std::vector<Dart> faces_cur;

    //create the faces - 2 cells and add them in an unordered_map
    Face cur_f = Face({f.a, f.b, f.c, f.d});
    int key_f = cur_f.face_concat(f.a, f.b, f.c,f.d); //create a unique key from the indices of the coordinates that form a face
    f_map.insert({key_f, id_f});


//        //create the 4 vertices - 0 cell
//        Vertex cur_v1=Vertex(vertices[f.a]);
//        Vertex cur_v2=Vertex(vertices[f.b]);
//        Vertex cur_v3=Vertex(vertices[f.c]);
//        Vertex cur_v4=Vertex(vertices[f.d]);


//
//        Point va = vertices[(f.a)].point;
//        Point vb = vertices[(f.b)].point;
//        Point vc = vertices[(f.c)].point;
//        Point vd = vertices[(f.d)].point;


    //std::string xyz_1=Vertex(vertices[f.a]).coords_concat(vertices[(f.a)].point);
    //std::cout <<xyz_1 <<std::endl;
    v_map.insert({Vertex(vertices[f.a]), f.a});


    //std::string xyz_2=Vertex(vertices[f.b]).coords_concat(vertices[(f.b)].point);
    v_map.insert({Vertex(vertices[f.b]), f.b});


    //std::string xyz_3=Vertex(vertices[f.c]).coords_concat(vertices[(f.c)].point);
    v_map.insert({Vertex(vertices[f.b]), f.c});

    //std::string xyz_4=Vertex(vertices[f.d]).coords_concat(vertices[(f.d)].point);
    v_map.insert({Vertex(vertices[f.b]), f.d});


    // create the 4 edges for each face - 1 cell
    Edge cur1 = Edge({id_e, f.a, f.b});
    int e1 = cur1.concat(f.a, f.b);
    int inv_e1 = cur1.inv_concat(f.a, f.b); //create the inverse edge (end, start)
    if (e_map.count(e1) == 0 && e_map.count(inv_e1) == 0) { //check for duplicates
      e_map.insert({e1, id_e});
      edges.push_back(cur1);

      Dart d1_1 = Dart({id_d, f.a, id_e, id_f, id_d + 1}); //f.a=vertex id
      darts.emplace_back(d1_1);
      faces_cur.emplace_back(d1_1);
      id_d++;
      Dart d1_2 = Dart({id_d, f.b, id_e, id_f, id_d - 1});
      darts.emplace_back(d1_2);
      faces_cur.emplace_back(d1_2);
      id_d++;

      id_e++;
    } else {
      if (e_map.count(e1) != 0) {
        id_e_exist = e_map.at(e1);
        Dart d1_1 = Dart({id_d, f.a, id_e_exist, id_f, id_d + 1});
        darts.emplace_back(d1_1);
        faces_cur.emplace_back(d1_1);
        id_d++;
        Dart d1_2 = Dart({id_d, f.b, id_e_exist, id_f, id_d - 1});
        darts.emplace_back(d1_2);
        faces_cur.emplace_back(d1_2);
        id_d++;
      }

      if (e_map.count(inv_e1) != 0) {
        id_e_exist = e_map.at(inv_e1);
        Dart d1_1 = Dart({id_d, f.a, id_e_exist, id_f, id_d + 1});
        darts.emplace_back(d1_1);
        faces_cur.emplace_back(d1_1);
        id_d++;
        Dart d1_2 = Dart({id_d, f.b, id_e_exist, id_f, id_d - 1});
        darts.emplace_back(d1_2);
        faces_cur.emplace_back(d1_2);
        id_d++;

      }
    }


    Edge cur2 = Edge({id_e, f.b, f.c});
    int e2 = cur2.concat(f.b, f.c);
    int inv_e2 = cur2.inv_concat(f.b, f.c);
    if (e_map.count(e2) == 0 && e_map.count(inv_e2) == 0) {
      e_map.insert({e2, id_e});
      edges.push_back(cur2);
      Dart d2_1 = Dart({id_d, f.b, id_e, id_f, id_d + 1});
      darts.emplace_back(d2_1);
      faces_cur.emplace_back(d2_1);
      id_d++;
      Dart d2_2 = Dart({id_d, f.c, id_e, id_f, id_d - 1});
      darts.emplace_back(d2_2);
      faces_cur.emplace_back(d2_2);
      id_d++;

      id_e++;
    } else {
      if (e_map.count(e2) != 0) {
        id_e_exist = e_map.at(e2);
        Dart d2_1 = Dart({id_d, f.b, id_e_exist, id_f, id_d + 1});
        darts.emplace_back(d2_1);
        faces_cur.emplace_back(d2_1);
        id_d++;
        Dart d2_2 = Dart({id_d, f.c, id_e_exist, id_f, id_d - 1});
        darts.emplace_back(d2_2);
        faces_cur.emplace_back(d2_2);
        id_d++;
      }

      if (e_map.count(inv_e2) != 0) {
        id_e_exist = e_map.at(inv_e2);
        Dart d2_1 = Dart({id_d, f.b, id_e_exist, id_f, id_d + 1});
        darts.emplace_back(d2_1);
        faces_cur.emplace_back(d2_1);
        id_d++;
        Dart d2_2 = Dart({id_d, f.c, id_e_exist, id_f, id_d - 1});
        darts.emplace_back(d2_2);
        faces_cur.emplace_back(d2_2);
        id_d++;

      }
    }

    Edge cur3 = Edge({id_e, f.c, f.d});
    int e3 = cur2.concat(f.c, f.d);
    int inv_e3 = cur3.inv_concat(f.c, f.d);
    if (e_map.count(e3) == 0 && e_map.count(inv_e3) == 0) {
      e_map.insert({e3, id_e});
      edges.push_back(cur3);
      Dart d3_1 = Dart({id_d, f.c, id_e, id_f, id_d + 1});
      darts.emplace_back(d3_1);
      faces_cur.emplace_back(d3_1);
      id_d++;
      Dart d3_2 = Dart({id_d, f.d, id_e, id_f, id_d - 1});
      darts.emplace_back(d3_2);
      faces_cur.emplace_back(d3_2);
      id_d++;

      id_e++;
    } else {
      if (e_map.count(e3) != 0) {
        id_e_exist = e_map.at(e3);
        Dart d2_1 = Dart({id_d, f.c, id_e_exist, id_f, id_d + 1});
        darts.emplace_back(d2_1);
        faces_cur.emplace_back(d2_1);
        id_d++;
        Dart d2_2 = Dart({id_d, f.d, id_e_exist, id_f, id_d - 1});
        darts.emplace_back(d2_2);
        faces_cur.emplace_back(d2_2);
        id_d++;
      }

      if (e_map.count(inv_e3) != 0) {
        id_e_exist = e_map.at(inv_e3);
        Dart d2_1 = Dart({id_d, f.c, id_e_exist, id_f, id_d + 1});
        darts.emplace_back(d2_1);
        faces_cur.emplace_back(d2_1);
        id_d++;
        Dart d2_2 = Dart({id_d, f.d, id_e_exist, id_f, id_d - 1});
        darts.emplace_back(d2_2);
        faces_cur.emplace_back(d2_2);
        id_d++;

      }
    }

    Edge cur4 = Edge({id_e, f.d, f.a});
    int e4 = cur4.concat(f.d, f.a);
    int inv_e4 = cur2.inv_concat(f.d, f.a);
    if (e_map.count(e4) == 0 && e_map.count(inv_e4) == 0) {
      e_map.insert({e4, id_e});
      edges.push_back(cur4);

      Dart d4_1 = Dart({id_d, f.d, id_e, id_f, id_d + 1});
      darts.emplace_back(d4_1);
      faces_cur.emplace_back(d4_1);
      id_d++;
      Dart d4_2 = Dart({id_d, f.a, id_e, id_f, id_d - 1});
      darts.emplace_back(d4_2);
      faces_cur.emplace_back(d4_2);
      id_d++;

      id_e++;
    } else {
      if (e_map.count(e4) != 0) {
        id_e_exist = e_map.at(e4);
        Dart d2_1 = Dart({id_d, f.d, id_e_exist, id_f, id_d + 1});
        darts.emplace_back(d2_1);
        faces_cur.emplace_back(d2_1);
        id_d++;
        Dart d2_2 = Dart({id_d, f.a, id_e_exist, id_f, id_d - 1});
        darts.emplace_back(d2_2);
        faces_cur.emplace_back(d2_2);
        id_d++;
      }

      if (e_map.count(inv_e4) != 0) {
        id_e_exist = e_map.at(inv_e4);
        Dart d2_1 = Dart({id_d, f.d, id_e_exist, id_f, id_d + 1});
        darts.emplace_back(d2_1);
        faces_cur.emplace_back(d2_1);
        id_d++;
        Dart d2_2 = Dart({id_d, f.a, id_e_exist, id_f, id_d - 1});
        darts.emplace_back(d2_2);
        faces_cur.emplace_back(d2_2);
        id_d++;

      }
    }


    for (auto i = darts.begin(); i != darts.end(); i++) {
      for (auto j: faces_cur) {
        if (i->e != j.e) {
          if (i->v == j.v) {
            if (i->f == j.f) {
              i->a1 = j.dart_id;
            }
          }

        }
      }
    }


    id_f++;
  }
  for (auto i = darts.begin(); i != darts.end(); i++) {
    for (auto j: darts) {
      if (i->e == j.e) {
        if (i->v == j.v) {
          if (i->f != j.f) {
            i->a2 = j.dart_id;
          }
        }

      }
    }
  }

  // ## Output generalised map to CSV ##
  std::ofstream output_darts("darts.csv");
  output_darts << "ID;a0;a1;a2;a3;v;e;f" << std::endl;
  for (auto i: darts) {
    output_darts << i.dart_id << ";";
    output_darts << i.a0 << ";";
    output_darts << i.a1 << ";";
    output_darts << i.a2 << ";";
    output_darts << i.a3 << ";";
    output_darts << (i.v) + 1
                 << ";"; // adding +1 so that the vertices id start from 1 and not for zero. in the main we used as the id the index, and indexes were starting from 0
    output_darts << i.e << ";";
    output_darts << i.f;
    output_darts << "\n";
  }

  output_darts.close();

  // ## Output vertices to CSV ##
  std::ofstream output_vertices("vertices.csv");
  output_vertices << "ID;dart;x;y;z" << std::endl;


  for (auto i: v_map) {
    int count_dart = 0;
    for (auto x: darts) {
      if (x.v == i.second && count_dart == 0) {
        count_dart++;
        output_vertices << (i.second) + 1 << ";";
        output_vertices << x.dart_id << ";";
        output_vertices << i.first.point.x << ";";
        output_vertices << i.first.point.y << ";";
        output_vertices << i.first.point.z;
        output_vertices << "\n";
      }
    }
  }

  output_vertices.close();

  // ## Output edges to CSV ##
  std::ofstream output_edges("edges.csv");
  output_edges << "ID;dart" << std::endl;

  for (auto i: e_map) {
    int count_dart = 0;
    for (auto x: darts) {
      if (x.e == i.second &&
          count_dart == 0) { //we dont need to add 1, because in the main we stored the indices in both cases
        count_dart++;
        output_edges << (i.second) << ";";
        output_edges << x.dart_id;
        output_edges << "\n";
      }
    }

  }
  output_edges.close();

  // ## Output faces to CSV ##
  std::ofstream output_faces("faces.csv");
  output_faces << "ID;dart" << std::endl;
  for (auto i: f_map) {
    int count_dart = 0; //we only need 1 dart per face, but every face has 8 darts in total, so we use a counter to only acquire 1 dart
    for (auto x: darts) {
      if (x.f == i.second && count_dart ==
                             0) { //x.f is the face id that was inserted in a dart and i.second is the unique id that was added in each face as a value
        count_dart++;
        output_faces << i.second << ";";
        output_faces << x.dart_id;
        output_faces << "\n";
      }
    }
  }
  output_faces.close();

  // ## Output volume to CSV ##
  std::ofstream output_volume("volume.csv");
  output_volume << "ID;dart" << std::endl;
  output_volume << "1"
                << ";"; //because we only have 1 object then the volume would have 1 object and we chose randomly one of the darts
  output_volume << "1";
  output_volume << "\n";

  output_volume.close();

  std::vector<int> torus_triang;
  std::vector<float> torus_vert;



  for (auto i = darts.begin(); i != darts.end(); i++) {

    std::cout << i->dart_id << std::endl;
    std::cout << vertices[i->v].point << std::endl; //vertex of the dart

    //create the barycentre of the edge for the dart
    //we do -1 because the ids start from 1, but we need the index so we want to start with 0
    //vertices[edges[d.e-1].start].point + vertices[edges[d.e-1].end].point)/2; // using the edge that corresponds to the dart according to gmap (darts vector), we get the coordinates of the starting point of the edge and then the ending point and divide it by 2
    std::cout << (vertices[edges[i->e - 1].start].point + vertices[edges[i->e - 1].end].point) / 2 << std::endl;

    std::cout <<  (vertices[(faces[i->f-1].a)].point +vertices[(faces[i->f-1].b)].point + vertices[(faces[i->f-1].c)].point + vertices[(faces[i->f-1].d)].point)/4 << std:: endl;

//        for (auto j: f_map) {
//            if (i->f == j.second) {
//                std::cout <<  (vertices[(faces[j.second-1].a)].point +vertices[(faces[j.second-1].b)].point + vertices[(faces[j.second-1].c)].point + vertices[(faces[j.second-1].d)].point)/4 << std:: endl;
//                Face f_bar = Face({faces[j.second - 1].a, faces[j.second - 1].b, faces[j.second - 1].c, faces[j.second - 1].d});
//                Point fbar = f_bar.face_baryc(vertices, faces[j.second - 1].a, faces[j.second - 1].b, faces[j.second - 1].c,
//                                              faces[j.second - 1].d);
//                std::cout << fbar << std::endl;
//            }
//       }
  }



//        //create the barycentre of the face for the dart
//        (vertices[faces[d.f-1].a].point + vertices[faces[d.f-1].b].point + vertices[faces[d.f-1].c].point + vertices[faces[d.f-1].d].point)/4;

  //std::cout <<  (vertices[(faces[d.f-1].a)-1].point +vertices[(faces[d.f-1].b)-1].point + vertices[(faces[d.f-1].c)-1].point + vertices[(faces[d.f-1].d)-1].point)/4 << std:: endl;


//    for (auto k : e_map) {
//        std::cout << k.first << " " << k.second<< " " << std::endl;
//    }
//
//    for (auto x : f_map) {
//        std::cout << x.first << " " << x.second<< " " << std::endl;
//    }

//    for (auto x : v_map) {
//        std::cout << x.first << " " << x.second<< " " << std::endl;
//    }

//for (auto d:darts){
//    std::cout<< " dartid " << d.dart_id << " A0 " << d.a0 << " A1 " << d.a1  << " A2 " << d.a2 << " vertex " << d.v<< " edge " <<d.e<< " face " <<d.f<<std::endl;
//}








  // ## Construct generalised map using the structures from Gmap.h ##





  // ## Create triangles from the darts ##

  // ## Write triangles to obj ##

  return 0;

}




