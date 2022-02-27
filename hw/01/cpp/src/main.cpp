// some standard libraries that are helpful for reading/writing text files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

#include "Gmap.h"

int main(int argc, const char * argv[]) {
    std::string file_in = "cube.obj";
    std::string file_out_obj = "D:/Geomatics/GEO_1004/geo1004_hw1/hw/01/data/torus_triangulated.obj";
    std::string file_out_csv_d = "D:/Geomatics/GEO_1004/geo1004_hw1/hw/01/data/torus_darts.csv";
    std::string file_out_csv_0 = "D:/Geomatics/GEO_1004/geo1004_hw1/hw/01/data/torus_vertices.csv";
    std::string file_out_csv_1 = "D:/Geomatics/GEO_1004/geo1004_hw1/hw/01/data/torus_edges.csv";
    std::string file_out_csv_2 = "D:/Geomatics/GEO_1004/geo1004_hw1/hw/01/data/torus_faces.csv";
    std::string file_out_csv_3 = "D:/Geomatics/GEO_1004/geo1004_hw1/hw/01/data/torus_volume.csv";

    // ## Read OBJ file ##

    std::ifstream stream_in;
    stream_in.open(file_in);

    // where to store loaded vertices, loaded faces
    std::vector<Vertex> vertices;
    std::vector<Dart> darts;
    std::vector<Edge> edges;
    std::vector<Face> faces;
    std::vector<Volume> volumes;
    std::vector<std::vector<int>> OBJ_faces;

    // Read the file
    if (stream_in.is_open()) {
        std::string line;
        while (getline(stream_in, line)) {
            std::istringstream iss(line);
            std::string word;
            iss >> word;
            if (word == "v") {
                std::vector<float> coordinates;
                while (iss >> word) coordinates.push_back(std::stof(word));
                if (coordinates.size() == 3) vertices.emplace_back(coordinates[0], coordinates[1], coordinates[2]);
                else vertices.push_back(Vertex());
            }
            if (word == "f") {
                std::vector<int> vertex_indexes;
                while (iss >> word) vertex_indexes.push_back(std::stoi(word)-1); //stoi converts str to int
                OBJ_faces.push_back(vertex_indexes);
            }
        }
        stream_in.close();
    }

    for (Vertex i: vertices) {
        std::cout << i.point << std::endl;

        }
        // ## Construct generalised map using the structures from Gmap.h ##

        // ## Output generalised map to CSV ##

        // ## Create triangles from the darts ##

        // ## Write triangles to obj ##

        return 0;
    }
