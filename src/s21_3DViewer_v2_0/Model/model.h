#ifndef S21_3DVIEWER2_0_MODEL_MODEL_H_
#define S21_3DVIEWER2_0_MODEL_MODEL_H_

#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace s21 {

enum axes { X, Y, Z };
enum projection { ORTHO, PERSPECTIVE };
enum type_edge { DOTTED, CONSTANT };
enum seting_verticle { NONE, CIRCLE, SQUARE };

/// @brief Структура точки
struct Point {
  float x;
  float y;
  float z;
};

class Model;  // опережающее объявление

class ModelDestroyer {
 public:
  ~ModelDestroyer();
  void initialize(Model *p) { p_instance_ = p; }

 private:
  Model *p_instance_ = nullptr;
};

class Model {
  friend class Normalization;
  friend class Transform;
  friend class Loader;

 public:
  /// @brief Получение ссылки на класс
  static Model *GetInstance();
  /// @brief Деструктор класса
  //~Model() { freeModel(); delete model_;}

  /// @brief Освобождение дианмической памяти
  void freeModel() noexcept;

  /// @brief Геттеры
  float *get_vertices() const noexcept { return vertices_; }
  unsigned int *get_indices() const noexcept { return indices_; }
  unsigned int get_amount_facets() const noexcept { return num_facets_; }
  unsigned int get_amount_vertices() const noexcept { return num_vertices_; }

  friend class ModelDestroyer;

 private:
  Model(){};
  ~Model(){};
  static Model *model_;
  static ModelDestroyer destroyer;
  float *vertices_ = nullptr;  // массив координат вершин x, y, z
  unsigned int *indices_ = nullptr;  // массив индексов вершин рёбер
  unsigned int num_vertices_ = 0;  // количество вершин
  unsigned int num_facets_ = 0;  // количество вершин граней
};

/// @brief Класс для нормализации модели
class Normalization {
 public:
  /// @brief Конструктор класса нормализации
  Normalization(Model *object) : model_(object){};

  /// @brief Метод включающий все методы нормализации
  void Normalize() noexcept;

 private:
  /// @brief Установка параметров модели для нормализации
  void SetParams() noexcept;
  /// @brief Помещение модели в центр координат
  void Centering() noexcept;
  /// @brief Изменение размера модели для отображения в окне
  void Sizing() noexcept;

  Model *model_;
  Point minCoords_;
  Point maxCoords_;
};

/// @brief Класс для преобразования модели
class Transform {
 public:
  /// @brief Конструктор класса transform
  /// @param model Указатель на model
  Transform(Model *model) : model_(model){};
  // void setModel(s21::Model *object) noexcept { model_ = object; }
  /// @brief Поворот модели
  /// @param angle Угол поворота в градусах
  /// @param axis Ось поворота
  void Rotate(float angle, axes axis) noexcept;

  /// @brief Масштабирование модели
  /// @param old_scale Старый масштаб
  /// @param new_scale Новый масштаб
  void Scale(float old_scale, float new_scale) noexcept;

  /// @brief Перемещение модели
  /// @param delta Смещение
  /// @param Axis Ось перемещения
  void Move(float delta, axes Axis) noexcept;

 private:
  Model *model_;
  /// @brief Перевод градусов в радианы
  /// @param angle угол в градусах из модели
  float DegreesToRadians(float angle) const noexcept {
    return angle * M_PI / 180;
  }
};

/// @brief Класс для загрузки модели
class Loader {
 public:
  /// @brief Загрузка модели из файла
  /// @param filePath Путь к файлу
  /// @param model Указатель на модель
  void loadObjModel(std::string filePath, Model *model);

 private:
  /// @brief Чтение и запись модели
  void LineReadingWriting();

  /*void MaxMinCord(); ВАЖНО! Теоретически можно получать максимальную
  и минимальную координату на этапе считывания, тогда не придется дваждый
  ходить по огромному массиву, пока сделею без этого
  */
  std::ifstream file_;
  Model *model_ = nullptr;
  std::vector<double> vertices_;
  std::vector<unsigned int> facets_;
};

}  // namespace s21

#endif  // S21_3DVIEWER2_0_MODEL_MODEL_H_
