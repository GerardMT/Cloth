#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace std;

class Mesh {
public:
    Mesh();

public:
    vector<float> vertices_;
    vector<int> faces_;
    vector<float> normals_;

    glm::vec3 min_;

    glm::vec3 max_;

    /**
     * @brief Reduce a set of vertices indices to a single or many vertices and adds them to the output mesh.
     * @param out_mesh Mesh to add vertices
     * @param result True if reduced
     * @param mehs Input mesh
     * @param indices Vertices indices of mesh to reduce
     * @param translationVertices New vertices indices of the out_mesh created vertices
     */
    typedef void (*reduce_function)(Mesh &out_mesh, bool &result, const Mesh &mesh, const vector<int> &indices, vector<int> &translationVertices);

    /**
     * @brief ReadFromPly Read the mesh stored in PLY format at the path filename
     * and stores the corresponding Mesh representation
     * @param filename The path to the PLY mesh.
     * @param mesh The resulting representation with computed per-vertex normals.
     * @return Whether it was able to read the file.
     */
    static void ReadFromPly(const string &filename, Mesh &mesh);
};

#endif // MESH_H
