#include "model.h"

namespace s21 {

Model* Model::model_ = nullptr;
ModelDestroyer Model::destroyer;

ModelDestroyer::~ModelDestroyer() {
  p_instance_->freeModel();
  delete p_instance_;
}

Model* Model::GetInstance() {
  if (model_ == nullptr) {
    model_ = new Model();
    destroyer.initialize(model_);
  }
  return model_;
}

void Model::freeModel() noexcept {
  if (vertices_ != nullptr) {
    delete[] vertices_;
    vertices_ = nullptr;
    num_vertices_ = 0;
  }
  if (indices_ != nullptr) {
    delete[] indices_;
    indices_ = nullptr;
    num_facets_ = 0;
  }
}

/*------class Normalization-------*/

void Normalization::Normalize() noexcept {
  SetParams();
  Centering();
  Sizing();
}

void Normalization::SetParams() noexcept {
  minCoords_.x = model_->vertices_[X];
  minCoords_.y = model_->vertices_[Y];
  minCoords_.z = model_->vertices_[Z];
  maxCoords_.x = model_->vertices_[X];
  maxCoords_.y = model_->vertices_[Y];
  maxCoords_.z = model_->vertices_[Z];
  for (unsigned int i = 0; i < model_->num_vertices_ * 3; i += 3) {
    if (model_->vertices_[i + X] < minCoords_.x)
      minCoords_.x = model_->vertices_[i + X];
    if (model_->vertices_[i + Y] < minCoords_.y)
      minCoords_.y = model_->vertices_[i + Y];
    if (model_->vertices_[i + Z] < minCoords_.z)
      minCoords_.z = model_->vertices_[i + Z];
    if (model_->vertices_[i + X] > maxCoords_.x)
      maxCoords_.x = model_->vertices_[i + X];
    if (model_->vertices_[i + Y] > maxCoords_.y)
      maxCoords_.y = model_->vertices_[i + Y];
    if (model_->vertices_[i + Z] > maxCoords_.z)
      maxCoords_.z = model_->vertices_[i + Z];
  }
}

void Normalization::Centering() noexcept {
  float x = minCoords_.x + (maxCoords_.x - minCoords_.x) / 2;
  float y = minCoords_.y + (maxCoords_.y - minCoords_.y) / 2;
  float z = minCoords_.z + (maxCoords_.z - minCoords_.z) / 2;

  for (unsigned int i = 0; i < model_->num_vertices_ * 3; i += 3) {
    model_->vertices_[i + X] -= x;
    model_->vertices_[i + Y] -= y;
    model_->vertices_[i + Z] -= z;
  }
}

void Normalization::Sizing() noexcept {
  float x = maxCoords_.x - minCoords_.x;
  float y = maxCoords_.y - minCoords_.y;
  float z = maxCoords_.z - minCoords_.z;
  float max = x;
  if (y > max) max = y;
  if (z > max) max = z;
  for (unsigned int i = 0; i < model_->num_vertices_ * 3; i += 3) {
    model_->vertices_[i + X] /= max;
    model_->vertices_[i + Y] /= max;
    model_->vertices_[i + Z] /= max;
  }
}

/*------class Transform-----*/

void Transform::Rotate(float angle, axes axis) noexcept {
  float x, y, z;
  float angleInRadians = DegreesToRadians(angle);
  for (unsigned int i = 0; i < model_->num_vertices_ * 3; i += 3) {
    x = model_->vertices_[i + X];
    y = model_->vertices_[i + Y];
    z = model_->vertices_[i + Z];
    switch (axis) {
      case X:
        model_->vertices_[i + Y] =
            y * cosf(angleInRadians) - z * sinf(angleInRadians);
        model_->vertices_[i + Z] =
            y * sinf(angleInRadians) + z * cosf(angleInRadians);
        break;
      case Y:
        model_->vertices_[i + X] =
            x * cosf(angleInRadians) + z * sinf(angleInRadians);
        model_->vertices_[i + Z] =
            -x * sinf(angleInRadians) + z * cosf(angleInRadians);
        break;
      case Z:
        model_->vertices_[i + X] =
            x * cosf(angleInRadians) - y * sinf(angleInRadians);
        model_->vertices_[i + Y] =
            x * sinf(angleInRadians) + y * cosf(angleInRadians);
        break;
    }
  }
}

void Transform::Scale(float old_scale, float new_scale) noexcept {
  for (unsigned int i = 0; i < model_->num_vertices_ * 3; i++) {
    model_->vertices_[i] *= new_scale / old_scale;
  }
}

void Transform::Move(float delta, axes Axis) noexcept {
  for (unsigned int i = 0; i < model_->num_vertices_ * 3; i += 3) {
    model_->vertices_[i + Axis] += delta;
  }
}

/*----class Loader----*/

void Loader::loadObjModel(std::string filePath, Model* model) {
  file_.open(filePath);
  if (!file_.is_open()) {
    throw std::logic_error("Can't open file");
  } else {
    model_ = model;
    LineReadingWriting();
    file_.close();
  }
}

void Loader::LineReadingWriting() {
  std::string readable_line = "";
  unsigned int first_facet_index = 0;
  double coordinate = 0.0;
  int i = 0, j = 0;

  std::string token, substr;
  std::string::size_type literal;
  while (std::getline(file_, readable_line)) {  // откуда, куда
    std::istringstream iss(readable_line);
    std::string prefix;
    iss >> prefix;
    if (prefix == "v") {
      while (j != 3 && iss >> coordinate) {
        // MaxMinCord(coordinate, static_cast<Coordinates>(j));
        vertices_.push_back(coordinate);
        j++;
      }
      j = 0;
    } else if (prefix == "f") {
      int vertexIndex;
      while (iss >> token) {
        literal = token.find_first_of('/');
        substr = token.substr(
            0, literal);  // if std::string::npos will be all string
        vertexIndex = std::stoi(substr);
        if (vertexIndex < 0) {
          vertexIndex += vertices_.size() / 3 + 1;
        }
        vertexIndex--;
        if (vertexIndex < 0 ||
            (unsigned int)vertexIndex >= vertices_.size() / 3)
          throw std::logic_error("Wrong vertex index");
        if (i == 0) {
          first_facet_index = vertexIndex;
          facets_.push_back(vertexIndex);
        } else {
          facets_.push_back(vertexIndex);
          facets_.push_back(vertexIndex);
        }
        i++;
      }
      facets_.push_back(first_facet_index);
      i = 0;
    }
  }
  i = 0;
  j = 0;
  // Вывести в отдельный метод
  model_->freeModel();
  model_->num_vertices_ = vertices_.size() / 3;
  model_->num_facets_ = facets_.size();
  model_->vertices_ = new float[vertices_.size()]();

  for (auto it : vertices_) {
    model_->vertices_[i] = it;
    i++;
  }

  model_->indices_ = new unsigned[facets_.size()]();
  for (auto it : facets_) {
    model_->indices_[j] = it;
    j++;
  }
  vertices_.clear();
  facets_.clear();
}

}  // namespace s21
