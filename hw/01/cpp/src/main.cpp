// By Eleni Theodoridou & Guilherme Spinoza Andreo
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <algorithm>

#include "Gmap.h"

// function to define the compiler in order to sort the unordered map for the triangle vertices
int comp(std::pair<Vertex,int> a,std::pair<Vertex,int> b) {
  return a.second < b.second;
}

int main(int argc, const char * argv[]) {
  std::string file_in = "./torus.obj";
  std::string file_out_obj = "torus_triangulated.obj";
  std::string file_out_csv_d = "output_darts.csv";
  std::string file_out_csv_0 = "torus_vertices.csv";
  std::string file_out_csv_1 = "output_edges.csv";
  std::string file_out_csv_2 = "torus_faces.csv";
  std::string file_out_csv_3 = "torus_volume.csv";

  // ## Read OBJ file ##

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

      //read the vertices of the file
      if (word == "v") {
        std::vector<float> coordinates;
        while (iss >> word) coordinates.push_back(std::stof(word)); // stof: converts string to float
        if (coordinates.size() == 3) vertices.emplace_back(coordinates[0], coordinates[1], coordinates[2]);
        else vertices.push_back(Vertex());
      }

      // ## read the faces ##
      if (word == "f") {
        std::vector<int> index;
        while (iss >> word) index.push_back(std::stoi(word)-1); //stoi converts str to int // we also substract 1 bc obj starts at 1 indexing and C++ structures start at 0 indexing
        if (index.size() == 4) init_faces.emplace_back(Face{index[0], index[1], index[2], index[3]});
      }
    }
  }

// ids for edges, face, vertices, volume and darts
  int id_e = 1;
  int id_f = 1;
  int id_d = 1;

// vector to store edges
  std::vector<Edge> edges;

// vector to store darts
  std::vector<Dart> darts;

// unordered maps for edges, vertices and faces to avoid duplicates
  std::unordered_map<std::string, int> e_map;
  std::unordered_map<Vertex, int> v_map;
  std::unordered_map<std::string, int> f_map;

// declaration to use to compare if the id of the edges already exists
  int id_e_exist;

//Looping through each face
  for (Face f: init_faces) {
    std::vector<Dart> faces_cur; //vector that contains only the darts of the current face. This defined here so that we have a new one for each face.

    //create the faces - 2 cells and add them in an unordered_map to check for duplicates
    Face cur_f = Face({f.a, f.b, f.c, f.d});
    std::string key_f = cur_f.face_concat(f.a, f.b, f.c,f.d); //create a unique key from the indices of the coordinates that form a face
    f_map.insert({key_f, id_f});

    // insert into the v_map the vertices. an unordered map is used to avoid duplicates
    v_map.insert({Vertex(vertices[f.a]), f.a});
    v_map.insert({Vertex(vertices[f.b]), f.b});
    v_map.insert({Vertex(vertices[f.b]), f.c});
    v_map.insert({Vertex(vertices[f.b]), f.d});


    // create the 4 edges for each face and then add them all to an unordered map(e_map) that does not have any duplicates
    Edge cur1 = Edge({id_e, f.a, f.b});
    std::string e1 = cur1.concat(f.a, f.b); //creates the current's edge key (start,end)
    std::string inv_e1 = cur1.inv_concat(f.a, f.b); //create the inverse edge key (end, start)
    if (e_map.count(e1) == 0 && e_map.count(inv_e1) == 0) { //check for duplicates
      e_map.insert({e1, id_e});
      edges.push_back(cur1);


      //define the dart that starts from the start point of the edge
      //define the a0 involution by assigning the next dart
      Dart d1_1 = Dart({id_d, f.a, id_e, id_f, id_d + 1}); //f.a=vertex id - start of the edge
      darts.emplace_back(d1_1);
      faces_cur.emplace_back(d1_1);
      id_d++;

      //define the dart that starts from the end point of the edge
      Dart d1_2 = Dart({id_d, f.b, id_e, id_f, id_d - 1}); //for the a0 involution we assign the previous dart
      darts.emplace_back(d1_2);
      faces_cur.emplace_back(d1_2);
      id_d++;
      id_e++;

    //the else statement is used to create the darts from an edge that is already in the map, so it does not need to be inserted again, but it has its own darts
    } else {
      if (e_map.count(e1) != 0) { //we first check for the same edge (start, end)
        id_e_exist = e_map.at(e1);  //use the id_e_exist to retrieve the id of the original edge that was inserted previously in the e_map
        Dart d1_1 = Dart({id_d, f.a, id_e_exist, id_f, id_d + 1});
        darts.emplace_back(d1_1);
        faces_cur.emplace_back(d1_1);
        id_d++;
        Dart d1_2 = Dart({id_d, f.b, id_e_exist, id_f, id_d - 1});
        darts.emplace_back(d1_2);
        faces_cur.emplace_back(d1_2);
        id_d++;
      }

      if (e_map.count(inv_e1) != 0) { //we check for the inverse edge (end, start)
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

    //repeat the same procedure as in Edge 1, but for the next edge
    Edge cur2 = Edge({id_e, f.b, f.c});
    std::string e2 = cur2.concat(f.b, f.c);
    std::string inv_e2 = cur2.inv_concat(f.b, f.c);
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

    //repeat the same procedure as in Edge 1
    Edge cur3 = Edge({id_e, f.c, f.d});
    std::string e3 = cur2.concat(f.c, f.d);
    std::string inv_e3 = cur3.inv_concat(f.c, f.d);
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

    //repeat the same procedure as in Edge 1
    Edge cur4 = Edge({id_e, f.d, f.a});
    std::string e4 = cur4.concat(f.d, f.a);
    std::string inv_e4 = cur2.inv_concat(f.d, f.a);
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

    //create the a1 involution, by iterating through the darts of the current face (as in a1 we have the same vertex and same face) and we check if 2 darts have the same vertex, face and different edge
    for (auto i: faces_cur) { //iterate through the darts
      for (auto j: faces_cur) {
        if (i.e != j.e) { //if dart is not on the same edge
          if (i.v == j.v) { // if dart is on the same vertex
            if (i.f == j.f) { // if dart is on the same face
              i.a1 = j.dart_id; // then this is the a1 involution
            }
          }

        }
      }
    }

    id_f++; //used to count the faces and the iterations
  }

  //finding the a2 involution, uses the same principle as before, but checks if the dart is not on the same face
  //use this outside of the faces loop, because we need all the darts and different face to find the a2 involution
  for (auto i : darts) {
    for (auto j: darts) {
      if (i.e == j.e) {
        if (i.v == j.v) {
          if (i.f != j.f) {
            i.a2 = j.dart_id;
          }
        }
      }
    }
  }


  // ## Output generalised map to CSV ##
  std::ofstream output_darts("torus_darts.csv");
  output_darts << "ID;a0;a1;a2;a3;v;e;f" << std::endl;
  for (auto i: darts) {
    output_darts << i.dart_id << ";";
    output_darts << i.a0 << ";";
    output_darts << i.a1 << ";";
    output_darts << i.a2 << ";";
    output_darts << i.a3 << ";";
    output_darts << (i.v) + 1
                 << ";"; // adding +1 so that the vertices' id starts from 1 and not for zero. in the main we used as the id the index, and indexes were starting from 0
    output_darts << i.e << ";";
    output_darts << i.f;
    output_darts << "\n";
  }
  output_darts.close();

  // ## Output vertices to CSV ##
  std::ofstream output_vertices("torus_vertices.csv");
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
  std::ofstream output_edges("torus_edges.csv");
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
  std::ofstream output_faces("torus_faces.csv");
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
  std::ofstream output_volume("torus_volume.csv");
  output_volume << "ID;dart" << std::endl;
  output_volume << "1"
                << ";"; //because we only have 1 object then the volume would have 1 object and we chose randomly one of the darts
  output_volume << "1";
  output_volume << "\n";

  output_volume.close();


  std::unordered_map<Vertex, int> torus_vert; //new unordered map to store the vertices of the torus,to avoid duplicates. will include the original vertices and the new vertices of the edge and face barycenters
  std::unordered_map<Tri_faces, int> triang; //unordered map to store the darts that are the faces of the torus

  int id_vt = 1; // id to store vertices from face barycenter and edge barycenter

  int vt_v=0; //id of the vertex of the dart
  int vt_e=0; //id of the barycenter of the edge of the dart
  int vt_f=0; //id of the barycenter of the face of the dart

  for (auto d:darts) {
    Vertex vert = vertices[d.v]; //original vertex of the dart

    //check if the vertex is already in the map and if not, add it by giving it a unique id.
    if (torus_vert.count(vert) == 0) {
      torus_vert.insert({vert, id_vt});
      vt_v = id_vt; //the unique id will be a value and will be used as an index
      id_vt++; // all the vertices that are added in the map need a unique id, so every time we add sth to the map the id is increased by 1
      } else { vt_v = torus_vert.find(vert)->second; } //in case the vertex already exists, we need to retrieve its value and to be used as index for the face

    //compute the barycenter of the face
    Face f_bar = Face({init_faces[d.f - 1].a, init_faces[d.f - 1].b, init_faces[d.f - 1].c,
                       init_faces[d.f - 1].d}); // create a face to call the function, based on that face
    Vertex fbar = f_bar.face_baryc(vertices, init_faces[d.f - 1].a, init_faces[d.f - 1].b, init_faces[d.f - 1].c,
                                   init_faces[d.f -1].d); // call the face_baryc function to compute the barycenter of the face

    //repeat the process we did for the original vertices, but for the faces
    if (torus_vert.count(fbar) == 0) {
      torus_vert.insert({fbar, id_vt}); //insert it into the unordered map of the vertices
      vt_f = id_vt;
      id_vt++;
    } else { vt_f = torus_vert.find(fbar)->second; }

    //compute the barycenter of the edge
    Edge e_bar = Edge({d.e, edges[d.e - 1].start, edges[d.e - 1].end});
    Vertex ebar = e_bar.edge_baryc(vertices, edges[d.e - 1].start, edges[d.e - 1].end);

    //repeat the process we did for the original vertices, but for the faces
    if (torus_vert.count(ebar) == 0) {
      torus_vert.insert({ebar, id_vt});
      vt_e = id_vt;
      id_vt++;
    } else { vt_e = torus_vert.find(ebar)->second; }
    //}


    //assign the vertices of the face. vertices are described counter clockwise.
    if (d.dart_id%2==0){
      Tri_faces f1 = Tri_faces({vt_e, vt_v, vt_f});
      triang.insert({f1,1}); //unordered map is used to avoid duplicates
    }
    else {  //faces that require a different order of the coordinates in order to have them in a counter clockwise order
      Tri_faces f2 = Tri_faces({vt_v, vt_e, vt_f});
      triang.insert({f2,2});
    }
  }

  //push the vertices from the unordered map into a vector and we sort it. this is needed for the correct output of the obj file.
  std::vector<std::pair<Vertex, int>> ordered(torus_vert.begin(), torus_vert.end());
  std::sort(ordered.begin(), ordered.end(), comp);


  //write the obj file
  std::ofstream output_triangulation("torus_triangulated.obj");

  //write the vertices first, as they are taken from the sorted vector
  for(int i=0;i<ordered.size();i++) {
    output_triangulation << "v" << " " << (ordered[i]).first.point.x << " " << (ordered[i]).first.point.y << " " << (ordered[i]).first.point.z << "\n";
  }

  //write the face
  for(auto t:triang) {
    output_triangulation << "f" << " " << t.first.a << " " << t.first.b << " " << t.first.c << "\n" ;
  }

  output_triangulation.close();


  return 0;

}




