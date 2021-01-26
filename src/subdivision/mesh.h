#pragma once
#include <vector>
#include <Eigen/Geometry>
#include <Eigen/Core>
#include <memory>
#include "utils/common_utils.h"
#include "base/model.h"

namespace SubDivision {
     using CommonUtils::index_t;

    struct Vertex{
        int id;  //Note: must consistent to its index
        Eigen::Vector3d p;
        std::vector<index_t> associated_edges;  //for updating vertex points
        std::vector<index_t> associated_polygons;  //for updating vertex points
    };
    
    struct Edge {
        index_t id1;
	    index_t id2;
        std::vector<index_t> associated_polygons;
    };

    struct Polygon {
        std::vector<index_t>  points; //for computing face points
        std::vector<index_t>  edges; //for connecting new face
        Eigen::Vector3d norm;

        size_t NumPoints() { return points.size();}
    };

    class Mesh {
public:
        void SetUp(const std::vector<Eigen::Vector3d>& vertices, 
                   const std::vector<std::vector<index_t>>& polygons);
        std::tuple<std::vector<float>, size_t> ConvertToTriangularMesh();

        static bool CommonVertex(const std::shared_ptr<Edge>& e1, const std::shared_ptr<Edge>& e2, index_t& vertex_id) {
            if(e1->id1 == e2->id1 || e1->id1 == e2->id2) {
                vertex_id = e1->id1;
                return true;
            } else if(e1->id2 == e2->id1 || e1->id2 == e2->id2) {
                vertex_id = e1->id2;
                return true;
            }
            return false;
        }

        inline const std::shared_ptr<Edge> EdgeElement(const int i) const {return edges_.at(i);}
        inline const std::shared_ptr<Vertex> VertexElement(const int i) const {return vertices_.at(i);}
        inline const Eigen::Vector3d& VertexPoint(const int i) const {return vertices_.at(i)->p;}

        inline const std::shared_ptr<Polygon> PolygonElement(const int i) const {return polygons_.at(i);}
        inline const std::vector<std::shared_ptr<Vertex>>& Vertices() const {return vertices_;}
        inline const std::vector<std::shared_ptr<Edge>>& Edges() const {return edges_;}
        inline const std::vector<std::shared_ptr<Polygon>>& Polygons() const {return polygons_;}
        void PrintPolygon();
        void PrintObj();
        void FeedNorm(const Model& model);

       

private:
         void ResetNorm();

        std::vector<std::shared_ptr<Vertex>> vertices_;
        std::vector<std::shared_ptr<Edge>> edges_;
        std::vector<std::shared_ptr<Polygon>> polygons_;
        const bool verbose = true;


    };
}
