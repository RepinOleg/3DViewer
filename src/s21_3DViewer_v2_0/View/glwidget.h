#ifndef S21_3DVIEWER_V2_0_GLWIDGET_H
#define S21_3DVIEWER_V2_0_GLWIDGET_H
#define GL_SILENCE_DEPRECATION

#include <QDebug>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QWidget>

#include "Controller/controller.h"
#include "Model/model.h"

//!  Основной класс, описывающий данные, необходимые для отрисовки модели
/*!
  Содержит данные, необходимые для: 1) хранения данных о модели 2) настройки
  отображения модели 3) координаты мыши относительно рабочего окна (для
  реализации вращения) и другие данные
*/

namespace Ui {
class GLWidget;
}

namespace s21 {

class GLWidget : public QOpenGLWidget {
  friend class MainWindow;
  Q_OBJECT

 public:
  explicit GLWidget(QWidget *parent = nullptr);
  ~GLWidget() = default;

 protected:
  void initializeGL() override;  //!< инициализация визуальной области
  void paintGL()
      override;  //!< создание видимой области и настройка матрицы координат

 private:
  void Draw();  //!< отрисовка модели по заданным настройкам
  void mousePressEvent(
      QMouseEvent *event) override;  //!< фиксация координат по нажатию мыши
  void mouseMoveEvent(
      QMouseEvent *event) override;  //!< изменение координат по движению мыши

  void ResetMovement();

  double fixed_angle_x_ = 0.0; /*!< фиксация угла поворота модели мышкой
                           (фиксируется после того, как мышка "отжата") */
  double fixed_angle_y_ = 0.0; /*!< фиксация угла поворота модели мышкой
                           (фиксируется после того, как мышка "отжата") */
  double width_lines_ = 1.0;  /*!< ширина линии */
  double width_dots_ = 1.0;   /*!< размер вершин */
  int type_lines_ = CONSTANT; /*!< тип линии */
  int paint_dot_ = NONE;      /*!< тип отрисовки вершин */
  int projectionType_ = PERSPECTIVE; /*!< тип проекции */
  float rLine_ = 0.0; /*!< r-значнение цвета ребра формации rgb */
  float gLine_ = 1.0; /*!< g-значнение цвета ребра формации rgb */
  float bLine_ = 0.0; /*!< b-значнение цвета ребра формации rgb */
  float rDot_ = 1.0; /*!< r-значнение цвета вершины формации rgb */
  float gDot_ = 0.0; /*!< g-значнение цвета вершины формации rgb */
  float bDot_ = 0.0; /*!< b-значнение цвета вершины формации rgb */
  float rBack_ = 0.0; /*!< r-значнение цвета фона формации rgb */
  float gBack_ = 0.0; /*!< g-значнение цвета фона формации rgb */
  float bBack_ = 1.0; /*!< b-значнение цвета фона формации rgb */
  float currentX_angle_ = 0.0;
  float currentY_angle_ = 0.0;
  float currentZ_angle_ = 0.0;
  float scaleM = 1.0; /*!< текущий масштаб */
  QPointF
      mousePosition_; /*!< текущее положение мыши (фиксируется после нажатия) */
  int rotationInitializied =
      1; /*!< фиксирование момента нажатия левой кнопкой мыши */
  double angle_x = 0; /*!< значение поворота модели в радианах по оси Х */
  double angle_y = 0; /*!< значение поворота модели в радианах по оси Y */
  Ui::GLWidget *ui_;
  Controller *controller_;
};

}  // namespace s21

#endif  // S21_3DVIEWER_V2_0_GLWIDGET_H
