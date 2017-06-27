#include "al/core/gl/al_GLEW.hpp"
#include "al/core/gl/al_VAOMesh.hpp"
#include <iostream>

using namespace al;

std::unordered_map<unsigned int, unsigned int> VAOMesh::mPrimMap = {
  { Mesh::POINTS, GL_POINTS },
  { Mesh::LINES, GL_LINES },
  { Mesh::LINE_STRIP, GL_LINE_STRIP },
  { Mesh::LINE_LOOP, GL_LINE_LOOP },
  { Mesh::TRIANGLES, GL_TRIANGLES },
  { Mesh::TRIANGLE_STRIP, GL_TRIANGLE_STRIP },
  { Mesh::TRIANGLE_FAN, GL_TRIANGLE_FAN },
  { Mesh::LINES_ADJACENCY, GL_LINES_ADJACENCY },
  { Mesh::LINE_STRIP_ADJACENCY, GL_LINE_STRIP_ADJACENCY },
  { Mesh::TRIANGLES_ADJACENCY, GL_TRIANGLES_ADJACENCY },
  { Mesh::TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY }
};

VAOMesh::VAOMesh() {
    vaoWrapper = std::make_shared<VAOWrapper>();
}

// when copying, make new vao
VAOMesh::VAOMesh(VAOMesh const& other): Mesh(other) {
    vaoWrapper = std::make_shared<VAOWrapper>();
    if (gl::loaded()) update();
    // std::cout << "copy ctor" << std::endl;
}

// when moving, move vao
VAOMesh::VAOMesh(VAOMesh&& other): Mesh(other) {
    vaoWrapper = other.vaoWrapper;
    // std::cout << "move ctor" << std::endl;
}

// when copying, make new vao
VAOMesh& VAOMesh::operator = (VAOMesh const& other) {
    copy(other);
    vaoWrapper = std::make_shared<VAOWrapper>();
    if (gl::loaded()) update();
    // std::cout << "copy assignment" << std::endl;
}

// when moving, move vao
VAOMesh& VAOMesh::operator = (VAOMesh&& other) {
    copy(other);
    vaoWrapper = other.vaoWrapper;
    // std::cout << "move assignment" << std::endl;
}

void VAOMesh::bind() {
  vao().bind();
}

void VAOMesh::unbind() {
  vao().unbind();
}

void VAOMesh::update() {
  vaoWrapper->GLPrimMode = mPrimMap[mPrimitive];
  vao().validate();
  vao().bind();
  updateAttrib(vertices(), positionAtt());
  updateAttrib(colors(), colorAtt());
  updateAttrib(texCoord2s(), texcoord2dAtt());
  updateAttrib(normals(), normalAtt());
  // updateAttrib(texCoord3s(), mTexcoord3dAtt);
  // updateAttrib(texCoord1s(), mTexcoord1dAtt);
  vao().unbind();
  if (indices().size() > 0) {
    if (!indexBuffer().created()) {
      // mIndexBuffer.create();
      // mIndexBuffer.bufferType(GL_ELEMENT_ARRAY_BUFFER);
      indexBuffer().create();
      indexBuffer().bufferType(GL_ELEMENT_ARRAY_BUFFER);
    }
    indexBuffer().bind();
    indexBuffer().data(
      sizeof(unsigned int) * indices().size(),
      indices().data()
    );
    indexBuffer().unbind();
  }
}

template <typename T>
void VAOMesh::updateAttrib(
  std::vector<T> const& data, MeshAttrib& att
) {
  // only enable attribs with content
  if (data.size() > 0) {
    vao().enableAttrib(att.index);
  }
  else {
    vao().disableAttrib(att.index);
    return;
  }

  // buffer yet created, make it and set vao to point to it
  if (!att.buffer.created()) {
    att.buffer.create();
    vao().attribPointer(att.index, att.buffer, att.size);
  }

  // upload CPU size data to buffer in GPU
  auto s = sizeof(T);
  att.buffer.bind();
  att.buffer.data(s * data.size(), data.data());
  att.buffer.unbind(); 
}

template void VAOMesh::updateAttrib<float>(
  std::vector<float> const& data, MeshAttrib& att
);

template void VAOMesh::updateAttrib<Vec2f>(
  std::vector<Vec2f> const& data, MeshAttrib& att
);

template void VAOMesh::updateAttrib<Vec3f>(
  std::vector<Vec3f> const& data, MeshAttrib& att
);

template void VAOMesh::updateAttrib<Vec4f>(
  std::vector<Vec4f> const& data, MeshAttrib& att
);

void VAOMesh::draw() {
  vao().bind();
  if (indices().size() > 0) {
    indexBuffer().bind();
    glDrawElements(vaoWrapper->GLPrimMode, indices().size(), GL_UNSIGNED_INT, NULL);
    indexBuffer().unbind();
  }
  else {
    glDrawArrays(vaoWrapper->GLPrimMode, 0, vertices().size());
  }
  vao().unbind();
}