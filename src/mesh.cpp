#include "mesh.h"

#include <eigen3/Eigen/Geometry>

#include <algorithm>
#include <limits>
#include <string>
#include <fstream>
#include <algorithm>
#include <set>
#include <vector>

Mesh::Mesh() {
    min_ = glm::vec3(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max());
    max_ = glm::vec3(numeric_limits<float>::lowest(), numeric_limits<float>::lowest(), numeric_limits<float>::lowest());
}

template <typename T>
void Add3Items(T i1, T i2, T i3, size_t index, vector<T> *vector) {
  (*vector)[index] = i1;
  (*vector)[index + 1] = i2;
  (*vector)[index + 2] = i3;
}

bool ReadPlyHeader(ifstream *fin, int *vertices, int *faces, bool &binary) {
  char line[100];

  fin->getline(line, 100);
  if (strncmp(line, "ply", 3) != 0) return false;

  *vertices = 0;
  fin->getline(line, 100);
  while (strncmp(line, "end_header", 10) != 0) {
    if (strncmp(line, "format", 6) == 0)  binary = strncmp(&line[7], "binary", 5) == 0;
    else if (strncmp(line, "element vertex", 14) == 0) *vertices = atoi(&line[15]);
    else if (strncmp(line, "element face", 12) == 0) *faces = atoi(&line[13]);
    fin->getline(line, 100);
  }

  if (*vertices <= 0) return false;

  return true;
}

void ReadPlyVerticesBinary(ifstream *fin, Mesh &mesh) {
  const size_t kVertices = mesh.vertices_.size() / 3;
  for (size_t i = 0; i < kVertices; ++i) {
    float x, y, z;
    fin->read(reinterpret_cast<char *>(&x), sizeof(float));
    fin->read(reinterpret_cast<char *>(&y), sizeof(float));
    fin->read(reinterpret_cast<char *>(&z), sizeof(float));

    Add3Items(x, y, z, i * 3, &(mesh.vertices_));
  }
}

void ReadPlyVerticesASCII(ifstream *fin, Mesh &mesh) {
  const size_t kVertices = mesh.vertices_.size() / 3;
  for (size_t i = 0; i < kVertices; ++i) {
    float x, y, z;

    *fin >> x;
    *fin >> y;
    *fin >> z;

    Add3Items(x, y, z, i * 3, &(mesh.vertices_));
  }
}

void ReadPlyFacesBinary(ifstream *fin, Mesh &mesh) {
  unsigned char vertex_per_face;

  const size_t kFaces = mesh.faces_.size() / 3;
  for (size_t i = 0; i < kFaces; ++i) {
    int v1, v2, v3;
    fin->read(reinterpret_cast<char *>(&vertex_per_face),
              sizeof(unsigned char));
    assert(vertex_per_face == 3);

    fin->read(reinterpret_cast<char *>(&v1), sizeof(int));
    fin->read(reinterpret_cast<char *>(&v2), sizeof(int));
    fin->read(reinterpret_cast<char *>(&v3), sizeof(int));
    Add3Items(v1, v2, v3, i * 3, &(mesh.faces_));
  }
}

void ReadPlyFacesASCII(ifstream *fin, Mesh &mesh) {
  unsigned int vertex_per_face;

  const size_t kFaces = mesh.faces_.size() / 3;
  for (size_t i = 0; i < kFaces; ++i) {
    int v1, v2, v3;
    *fin >> vertex_per_face;
    assert(vertex_per_face == 3);

    *fin >> v1;
    *fin >> v2;
    *fin >> v3;

    Add3Items(v1, v2, v3, i * 3, &(mesh.faces_));
  }
}


void ComputeVertexNormals(const vector<float> &vertices,
                          const vector<int> &faces,
                          vector<float> *normals) {
  const size_t kFaces = faces.size();
  vector<float> face_normals(kFaces, 0);

  for (size_t i = 0; i < kFaces; i += 3) {
    Eigen::Vector3d v1(vertices[faces[i] * 3], vertices[faces[i] * 3 + 1],
                       vertices[faces[i] * 3 + 2]);
    Eigen::Vector3d v2(vertices[faces[i + 1] * 3],
                       vertices[faces[i + 1] * 3 + 1],
                       vertices[faces[i + 1] * 3 + 2]);
    Eigen::Vector3d v3(vertices[faces[i + 2] * 3],
                       vertices[faces[i + 2] * 3 + 1],
                       vertices[faces[i + 2] * 3 + 2]);
    Eigen::Vector3d v1v2 = v2 - v1;
    Eigen::Vector3d v1v3 = v3 - v1;
    Eigen::Vector3d normal = v1v2.cross(v1v3);

    if (normal.norm() < 0.00001) {
      normal = Eigen::Vector3d(0.0, 0.0, 0.0);
    } else {
      normal.normalize();
    }

    for (size_t j = 0; j < 3; ++j) face_normals[i + j] = normal[j];
  }

  const size_t kVertices = vertices.size();
  normals->resize(kVertices, 0);
  for (size_t i = 0; i < kFaces; i += 3) {
    for (size_t j = 0; j < 3; ++j) {
      size_t idx = static_cast<size_t>(faces[i + j]);
      Eigen::Vector3d v1(vertices[faces[i + j] * 3],
                         vertices[faces[i + j] * 3 + 1],
                         vertices[faces[i + j] * 3 + 2]);
      Eigen::Vector3d v2(vertices[faces[i + (j + 1) % 3] * 3],
                         vertices[faces[i + (j + 1) % 3] * 3 + 1],
                         vertices[faces[i + (j + 1) % 3] * 3 + 2]);
      Eigen::Vector3d v3(vertices[faces[i + (j + 2) % 3] * 3],
                         vertices[faces[i + (j + 2) % 3] * 3 + 1],
                         vertices[faces[i + (j + 2) % 3] * 3 + 2]);

      Eigen::Vector3d v1v2 = v2 - v1;
      Eigen::Vector3d v1v3 = v3 - v1;
      double angle = acos(v1v2.dot(v1v3) / (v1v2.norm() * v1v3.norm()));

      if (angle == angle) {
        for (size_t k = 0; k < 3; ++k) {
          (*normals)[idx * 3 + k] += face_normals[i + k] * angle;
        }
      }
    }
  }

  const size_t kNormals = normals->size();
  for (size_t i = 0; i < kNormals; i += 3) {
    Eigen::Vector3d normal((*normals)[i], (*normals)[i + 1], (*normals)[i + 2]);
    if (normal.norm() > 0) {
      normal.normalize();
    } else {
      normal = Eigen::Vector3d(0, 0, 0);
    }

    for (size_t j = 0; j < 3; ++j) (*normals)[i + j] = normal[j];
  }
}

void ComputeBoundingBox(const vector<float> &vertices, Mesh &mesh) {
  const size_t kVertices = vertices.size() / 3;
  for (size_t i = 0; i < kVertices; ++i) {
    mesh.min_[0] = min(mesh.min_[0], vertices[i * 3]);
    mesh.min_[1] = min(mesh.min_[1], vertices[i * 3 + 1]);
    mesh.min_[2] = min(mesh.min_[2], vertices[i * 3 + 2]);

    mesh.max_[0] = max(mesh.max_[0], vertices[i * 3]);
    mesh.max_[1] = max(mesh.max_[1], vertices[i * 3 + 1]);
    mesh.max_[2] = max(mesh.max_[2], vertices[i * 3 + 2]);
  }
}

void Mesh::ReadFromPly(const string &filename, Mesh &mesh) {
  ifstream fin;

  fin.open(filename.c_str(), ios_base::in | ios_base::binary);
  if (!fin.is_open() || !fin.good()) throw 1;

  int vertices = 0, faces = 0;
  bool binary;
  if (!ReadPlyHeader(&fin, &vertices, &faces, binary)) {
    fin.close();
    throw 1;
  }

  mesh.vertices_.resize(static_cast<size_t>(vertices) * 3);
  if (binary) {
    ReadPlyVerticesBinary(&fin, mesh);
  } else {
    ReadPlyVerticesASCII(&fin, mesh);
  }

  mesh.faces_.resize(static_cast<size_t>(faces) * 3);
  if (binary) {
    ReadPlyFacesBinary(&fin, mesh);
  } else {
    ReadPlyFacesASCII(&fin, mesh);
  }

  fin.close();

  ComputeVertexNormals(mesh.vertices_, mesh.faces_, &mesh.normals_);
  ComputeBoundingBox(mesh.vertices_, mesh);
}
