#include "glwidget.h"

namespace s21 {

GLWidget::GLWidget(QWidget *parent) { parent->setVisible(true); }

void GLWidget::initializeGL() { glClearColor(rBack_, gBack_, bBack_, 1); }

void GLWidget::paintGL() {
  glClearColor(rBack_, gBack_, bBack_, 1);  // background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Эти параметры задают ограничения для экрана, точки нормализуются
  // относительно этих границ
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (projectionType_ == PERSPECTIVE) {
    glFrustum(-1, 1, -1, 1, 1, 99999);
    glTranslatef(0, 0, -2.6);
  } else if (projectionType_ == ORTHO) {
    glOrtho(-1.5, 1.5, -1.5, 1.5, -2, 1000);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glRotatef(angle_x, 1, 0, 0);
  glRotatef(angle_y, 0, 1, 0);
  if (controller_->get_vertices() !=
      NULL) {  // Если модель не задана, не рисуем ничего
    Draw();
  }
}

void GLWidget::Draw() {
  glEnableClientState(
      GL_VERTEX_ARRAY);  // разрешение использования массива вершин
  glVertexPointer(3, GL_FLOAT, 0,
                  controller_->get_vertices());  // указатель на массив вершин
  if (type_lines_ == DOTTED) {
    glEnable(GL_LINE_STIPPLE);  // рисование пунктиром
    glLineStipple(1, 0x00F0);
  } else {
    glDisable(GL_LINE_STIPPLE);  // возврат линий
  }
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // установка режима рисования
  glLineWidth(width_lines_);  // установка ширины линии
  glColor3f(rLine_, gLine_, bLine_);  // установка цвета
  glDrawElements(GL_LINES, controller_->get_amount_facets(), GL_UNSIGNED_INT,
                 controller_->get_indices());  // рисование линий

  if (paint_dot_ > 0) {  // Рисовать ли точки
    if (paint_dot_ == 1)
      glEnable(GL_POINT_SMOOTH);  // Круг
    else
      glDisable(GL_POINT_SMOOTH);  // Квадрат
    glPointSize(width_dots_);
    glColor3d(rDot_, gDot_, bDot_);
    glDrawArrays(GL_POINTS, 0, controller_->get_amount_vertices());
  }
  glDisable(GL_LINE_STIPPLE);
  glDisable(GL_POINT_SMOOTH);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  // fixed_angle_x_ = 0.0;
  // fixed_angle_y_ = 0.0;
  if (event->buttons() & Qt::LeftButton) {
    rotationInitializied = 1;
    mousePosition_ =
        event->position();  // если зажата левая клавиша - вращаем относительно
                            // предыдущей ("повернутой") позиции
  } else if (event->buttons() & Qt::RightButton) {
    angle_x = 0.;
    angle_y = 0.;
    update();
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (this->rotationInitializied) {
    fixed_angle_y_ = angle_y;
    fixed_angle_x_ = angle_x;
    this->rotationInitializied = 0;
  }
  angle_x =
      fixed_angle_x_ +
      1 / M_PI * (event->pos().y() - mousePosition_.y());  // вывести на экран!
  angle_y = fixed_angle_y_ + 1 / M_PI * (event->pos().x() - mousePosition_.x());
  update();
}

void GLWidget::ResetMovement() {
  currentX_angle_ = 0;
  angle_x = 0;
  currentY_angle_ = 0;
  angle_y = 0;
  currentZ_angle_ = 0;
  scaleM = 1;
}

}  // namespace s21
