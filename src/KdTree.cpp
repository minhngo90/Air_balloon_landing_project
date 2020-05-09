//  KdTree Template - Simple KdTree class
//
//  SJSU  - CS134 Game Development
//
//  Kevin M. Smith   04/19/20

//  **Important:  Vertices (x, y, z) in the mesh are stored in the Tree node as an integer index.
//  to read the 3D vertex point from the mesh given index i,  use the function ofMesh::getVertex(i);  See
//  KdTree::meshBounds() for an example of usage;
//
//

#include "KdTree.h"
 
// draw KdTree (recursively)
//
void KdTree::draw(TreeNode & node, int numLevels, int level) {
    if (level >= numLevels) return;
    drawBox(node.box);
    level++;
    for (int i = 0; i < node.children.size(); i++) {
        draw(node.children[i], numLevels, level);
    }
}

// draw only leaf Nodes
//
void KdTree::drawLeafNodes(TreeNode & node) {
    if (node.children.size() == 0 && node.points.size()>0 ) drawBox(node.box);
    
    for (int i = 0; i < node.children.size(); i++) {
        drawLeafNodes(node.children[i]);
        
    }
}


//draw a box from a "Box" class
//
void KdTree::drawBox(const Box &box) {
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
    float w = size.x();
    float h = size.y();
    float d = size.z();
    ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box KdTree::meshBounds(const ofMesh & mesh) {
    int n = mesh.getNumVertices();
    ofVec3f v = mesh.getVertex(0);
    ofVec3f max = v;
    ofVec3f min = v;
    for (int i = 1; i < n; i++) {
        ofVec3f v = mesh.getVertex(i);

        if (v.x > max.x) max.x = v.x;
        else if (v.x < min.x) min.x = v.x;

        if (v.y > max.y) max.y = v.y;
        else if (v.y < min.y) min.y = v.y;

        if (v.z > max.z) max.z = v.z;
        else if (v.z < min.z) min.z = v.z;
    }
    cout << "vertices: " << n << endl;
//    cout << "min: " << min << "max: " << max << endl;
    return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

// getMeshPointsInBox:  return an array of indices to points in mesh that are contained
//                      inside the Box.  Return count of points found;
//
int KdTree::getMeshPointsInBox(const ofMesh & mesh, const vector<int>& points,
    Box & box, vector<int> & pointsRtn)
{
    int count = 0;
    for (int i = 0; i < points.size(); i++) {
        ofVec3f v = mesh.getVertex(points[i]);
        if (box.inside(Vector3(v.x, v.y, v.z))) {
            count++;
            pointsRtn.push_back(points[i]);
        }
    }
    return count;
}



//  Subdivide a Box; return children in  boxList
//
void KdTree::subDivideBox(const Box &box, vector<Box> & boxList) {
    
//    Vector3 min = box.parameters[0];
//    Vector3 max = box.parameters[1];
//    Vector3 size = max - min;
//    Vector3 center = size / 2 + min;
//    float xdist = (max.x() - min.x()) / 2;
//    float ydist = (max.y() - min.y()) / 2;
//    float zdist = (max.z() - min.z()) / 2;
//
//    boxList.clear();
//    if (xdist >= ydist && xdist >= zdist) {
//        Box box1 = Box(min, Vector3((max.x() + min.x()) / 2, max.y(), max.z()));
//        Box box2 = Box(Vector3((max.x() + min.x()) / 2, min.y(), min.z()), max);
//        boxList.push_back(box1);
//        boxList.push_back(box2);
//
//    }
//    if (ydist >= xdist && ydist >= zdist) {
//        Box box1 = Box(min, Vector3(max.x(), (max.y() + min.y()) / 2, max.z()));
//        Box box2 = Box(Vector3(min.x(), (max.y() + min.y()) / 2, min.z()), max);
//        boxList.push_back(box1);
//        boxList.push_back(box2);
//
//    }
//    if (zdist >= xdist && zdist >= ydist) {
//        Box box1 = Box(min, Vector3(max.x(), max.y(), (max.z() + min.z()) / 2));
//        Box box2 = Box(Vector3(min.x(), min.y(), (max.z() + min.z()) / 2), max);
//        boxList.push_back(box1);
//        boxList.push_back(box2);
//
//    }
    
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    float xdist = (max.x() - min.x()) / 2;
    float ydist = (max.y() - min.y()) / 2;
    float zdist = (max.z() - min.z()) / 2;
    Vector3 h = Vector3(0, ydist, 0);

    //  generate ground floor
    //
    Box b[8];
    b[0] = Box(min, center);
    b[1] = Box(b[0].min() + Vector3(xdist, 0, 0), b[0].max() + Vector3(xdist, 0, 0));
    b[2] = Box(b[1].min() + Vector3(0, 0, zdist), b[1].max() + Vector3(0, 0, zdist));
    b[3] = Box(b[2].min() + Vector3(-xdist, 0, 0), b[2].max() + Vector3(-xdist, 0, 0));

    boxList.clear();
    for (int i = 0; i < 4; i++)
        boxList.push_back(b[i]);

    // generate second story
    //
    for (int i = 4; i < 8; i++) {
        b[i] = Box(b[i - 4].min() + h, b[i - 4].max() + h);
        boxList.push_back(b[i]);
    }

    
}

void KdTree::create(const ofMesh & geo, int numLevels) {
    // initialize KdTree structure
    //
    mesh = geo;
    int level = 0;

    root.box = meshBounds(mesh);
    for (int i = 0; i < mesh.getNumVertices(); i++) {
        root.points.push_back(i);
    }
    level++;

    subdivide(mesh, root, numLevels, level);

}

void KdTree::subdivide(const ofMesh & mesh, TreeNode & node, int numLevels, int level) {
    if (level >= numLevels) return;
    
    // subdivide this node
    //
    
    vector<Box> boxList;
    subDivideBox(node.box, boxList);
    
    level++;
    
    for (int i = 0; i < boxList.size(); i++) {
        TreeNode child;
        child.box = boxList[i];
        node.children.push_back(child);
        if (getMeshPointsInBox(mesh, node.points, node.children[i].box, node.children[i].points) > 0) {
            
            subdivide(mesh, node.children[i], numLevels, level);
        }
    }
    
}

bool KdTree::intersect(const Ray &ray, const TreeNode & node, TreeNode & nodeRtn) {
    
    for (int i = 0; i < node.children.size(); i++) {
        if (node.children[i].children.size() == 0 && node.children[i].points.size() > 0) {
            if (node.children[i].box.intersect(ray, 0, 1000)) {
                nodeRtn = node.children[i];
                //cout << "hit leaf" << endl;
                return true;
            }
        }
        bool hit = intersect(ray, node.children[i], nodeRtn);
    }
}


bool KdTree::intersect(const Box &box, TreeNode & node, vector<Box> & boxListRtn) {
    for (int i = 0; i < node.children.size(); i++) {
        if (node.children[i].children.size() == 0 && node.children[i].points.size() > 0) {
            if (node.children[i].box.overlap(box)) {
                boxListRtn.push_back(node.children[i].box);
                //cout << "hit leaf" << endl;
                return true;
            }
        }
        bool hit = intersect(box, node.children[i], boxListRtn);
    }
}
