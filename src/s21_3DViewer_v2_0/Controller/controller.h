#ifndef S21_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H_
#define S21_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H_

#include "../Model/model.h"

namespace s21 {
/// @brief класс для разделения model и view
class Controller {
 public:
  /// @brief Базовый конструктор
  /// @param object Указатель на объект класса Model
  Controller(Model* object)
      : object_(object), normalize_(object), transform_(object) {}

  /// @brief освобождение выделенной памяти в классе Model
  void freeModel() noexcept { object_->freeModel(); }

  /// @brief Загрузка модели
  /// @param filePath Путь к файлу
  void loadObjModel(std::string filePath) {
    loader_.loadObjModel(filePath, object_);
  }
  /// @brief Нормализация модели
  void Normalize() noexcept { normalize_.Normalize(); }

  /// @brief Геттер
  /// @return Возвращает количество вершин
  unsigned int get_amount_vertices() const noexcept {
    return object_->get_amount_vertices();
  }

  /// @brief Геттер
  /// @return Возвращает количество вершин граней
  unsigned int get_amount_facets() const noexcept {
    return object_->get_amount_facets();
  }

  /// @brief Геттер
  /// @return Возвращает указатель на массив точек
  float* get_vertices() const noexcept { return object_->get_vertices(); }

  /// @brief Геттер
  /// @return Вовзращает указатель на массив индексов вершин рёбер
  unsigned int* get_indices() const noexcept { return object_->get_indices(); }

  /// @brief Метод для передвижения модели
  /// @param delta Величина сдвига
  /// @param Axis Ось перемещения
  void Move(float delta, axes Axis) noexcept { transform_.Move(delta, Axis); }

  /// @brief Поворот модели
  /// @param angle Угол поворота в градусах
  /// @param axis Ось поворота
  void Rotate(float angle, axes Axis) noexcept {
    transform_.Rotate(angle, Axis);
  }

  /// @brief Масштабирование модели
  /// @param old_scale Старый масштаб
  /// @param new_scale Новый масштаб
  void Scale(float old_scale, float new_scale) noexcept {
    transform_.Scale(old_scale, new_scale);
  }

 private:
  s21::Model* object_;
  s21::Normalization normalize_;
  s21::Loader loader_;
  s21::Transform transform_;
};
}  // namespace s21

#endif  // S21_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H_
