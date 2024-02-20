#include "mainwindow.h"

#include "ui_mainwindow.h"

namespace s21 {
MainWindow::MainWindow(s21::Controller *control, QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), controller_(control) {
  ui_->setupUi(this);

  glInterface_ = ui_->viewport;
  glInterface_->controller_ = control;

  // убираем стрелки у спинБоксов и делаем десятичную точку вместо запятой
  ui_->rotateXInput->setButtonSymbols(QAbstractSpinBox::NoButtons);
  ui_->rotateXInput->setLocale(QLocale::C);
  ui_->rotateYInput->setButtonSymbols(QAbstractSpinBox::NoButtons);
  ui_->rotateYInput->setLocale(QLocale::C);
  ui_->rotateZInput->setButtonSymbols(QAbstractSpinBox::NoButtons);
  ui_->rotateZInput->setLocale(QLocale::C);
  ui_->scaleSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
  ui_->edgeWidth_spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
  ui_->edgeWidth_spinbox->setLocale(QLocale::C);
  ui_->verticeSize_spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
  ui_->verticeSize_spinbox->setLocale(QLocale::C);
  ui_->moveXInput->setButtonSymbols(QAbstractSpinBox::NoButtons);
  ui_->moveXInput->setLocale(QLocale::C);
  ui_->moveYInput->setButtonSymbols(QAbstractSpinBox::NoButtons);
  ui_->moveYInput->setLocale(QLocale::C);
  ui_->moveZInput->setButtonSymbols(QAbstractSpinBox::NoButtons);
  ui_->moveZInput->setLocale(QLocale::C);

  // добавляем радиобаттоны в группы и включаем по умолчанию
  // тип проекции
  QButtonGroup *groupProjection = new QButtonGroup(this);
  groupProjection->addButton(ui_->radioButton_orthogonal);
  groupProjection->addButton(ui_->radioButton_perspective);
  ui_->radioButton_perspective->toggle();

  // тип линий
  QButtonGroup *groupEdges = new QButtonGroup(this);
  groupEdges->addButton(ui_->radioButton_constant);
  groupEdges->addButton(ui_->radioButton_dotted);
  ui_->radioButton_constant->toggle();

  // тип отображения вершин
  QButtonGroup *groupVertices = new QButtonGroup(this);
  groupVertices->addButton(ui_->radioButton_noneVertice);
  groupVertices->addButton(ui_->radioButton_circleVertice);
  groupVertices->addButton(ui_->radioButton_squareVertice);
  ui_->radioButton_noneVertice->toggle();

  // формат для сохранения изображения
  QButtonGroup *saveFormat = new QButtonGroup(this);
  saveFormat->addButton(ui_->radioButton_jpgFormat);
  saveFormat->addButton(ui_->radioButton_bmpFormat);
  ui_->radioButton_jpgFormat->toggle();
  inputhandle_ = new InputHandler(this);
  subject_ = new Subject(this);
  applySettings();
  SetColors();
}

/// @brief Деструктор
MainWindow::~MainWindow() {
  saveSettings();
  delete glInterface_;
  delete ui_;
  delete inputhandle_;
  delete subject_;
}

///@brief Открытие файла
void MainWindow::on_open_file_clicked() {
  QString filename =
      QFileDialog::getOpenFileName(this, "Choose a file", "./", "*.obj");
  ui_->label_filepathInfo->setText("");
  ui_->label_verticesInfo->setText("");
  ui_->label_edgesNumber->setText("");
  bool error = false;
  try {
    controller_->loadObjModel(filename.toStdString());
  } catch (std::runtime_error &e) {
    throwError(e.what());
    error = true;
    controller_->freeModel();
  } catch (std::logic_error &e) {
    throwError(e.what());
    error = true;
    controller_->freeModel();
  }
  if (!error) {
    controller_->Normalize();
    filename.chop(4);
    ui_->label_filepathInfo->setText(
        filename.sliced(filename.lastIndexOf("/") + 1));
    ui_->label_verticesInfo->setText(
        QString::number(controller_->get_amount_vertices()));
    ui_->label_edgesNumber->setText(
        QString::number(controller_->get_amount_facets() / 2));
    resetMovement();
  }
  glInterface_->update();
}

///@brief Сброс поворотов, движения, масштаба модели
void MainWindow::resetMovement() {
  ui_->viewport->ResetMovement();
  ui_->rotateXInput->setValue(0);
  ui_->rotateYInput->setValue(0);
  ui_->rotateZInput->setValue(0);
  ui_->scaleSpinBox->setValue(100);
  ui_->moveXInput->setValue(0);
  moveX_ = 0;
  ui_->moveYInput->setValue(0);
  moveY_ = 0;
  ui_->moveZInput->setValue(0);
  moveZ_ = 0;
}

///@brief Очищение динамической памяти, выделенной для модели
void MainWindow::on_MainWindow_destroyed() { controller_->freeModel(); }

/*-----------Команда для вращения может быть---------------*/
/**
 * @brief Вращение вокруг оси X слайдером
 * @param[in] value значение перемещения по Х
 */
void MainWindow::on_rotateXInput_valueChanged(double arg1) {
  ui_->rotationX_slider->setValue(ui_->rotateXInput->value());
  rotateModel(X, (float)arg1);
}

/**
 * @brief Вращение вокруг оси X спинбоксом
 * @param[in] value значение перемещения по Х
 */
void MainWindow::on_rotationX_slider_valueChanged(int value) {
  ui_->rotateXInput->setValue((double)value);
  rotateModel(X, (float)value);
}

/**
 * @brief Вращение вокруг оси Y слайдером
 * @param[in] value значение перемещения по Y
 */
void MainWindow::on_rotateYInput_valueChanged(double arg1) {
  ui_->rotationY_slider->setValue(ui_->rotateYInput->value());
  rotateModel(Y, (float)arg1);
}

/**
 * @brief Вращение вокруг оси Y спинбоксом
 * @param[in] value значение перемещения по Y
 */
void MainWindow::on_rotationY_slider_valueChanged(int value) {
  ui_->rotateYInput->setValue((double)value);
  rotateModel(Y, (float)value);
}

/**
 * @brief Вращение вокруг оси Z слайдером
 * @param[in] value значение перемещения по Z
 */
void MainWindow::on_rotateZInput_valueChanged(double arg1) {
  ui_->rotationZ_slider->setValue(ui_->rotateZInput->value());
  rotateModel(Z, (float)arg1);
}

/**
 * @brief Вращение вокруг оси Z спинбоксом
 * @param[in] value значение перемещения по Z
 */
void MainWindow::on_rotationZ_slider_valueChanged(int value) {
  ui_->rotateZInput->setValue((double)value);
  rotateModel(Z, (float)value);
}

/**
 * @brief Перемещение по оси X через ввод
 * @param[in] arg1 значение перемещения по X
 */
void MainWindow::on_moveXInput_valueChanged(double arg1) {
  if (controller_->get_vertices()) {
    controller_->Move(((float)arg1 - moveX_) / 100, X);
    glInterface_->update();
  }
  moveX_ = (float)arg1;
}

/**
 * @brief Перемещение по оси Y через ввод
 * @param[in] arg1 значение перемещения по Y
 */
void MainWindow::on_moveYInput_valueChanged(double arg1) {
  if (controller_->get_vertices()) {
    controller_->Move(((float)arg1 - moveY_) / 100, Y);
    glInterface_->update();
  }
  moveY_ = (float)arg1;
}

/**
 * @brief Перемещение по оси Z через ввод
 * @param[in] arg1 значение перемещения по Z
 */
void MainWindow::on_moveZInput_valueChanged(double arg1) {
  if (controller_->get_vertices()) {
    controller_->Move(((float)arg1 - moveZ_) / 100, Z);
    glInterface_->update();
  }
  moveZ_ = (float)arg1;
}

/**
 * @brief Вращение модели
 * @param[in] axes тип осей
 * @param[in] angle - значение угла поворота
 */
void MainWindow::rotateModel(axes axis, float angle) {
  float rotation = 0;
  switch (axis) {
    case X:
      rotation = angle > glInterface_->currentX_angle_
                     ? angle - glInterface_->currentX_angle_
                     : -(glInterface_->currentX_angle_ - angle);
      controller_->Rotate(rotation, X);
      ui_->viewport->currentX_angle_ += rotation;
      break;
    case Y:
      rotation = angle > glInterface_->currentY_angle_
                     ? angle - glInterface_->currentY_angle_
                     : -(glInterface_->currentY_angle_ - angle);

      controller_->Rotate(rotation, Y);
      ui_->viewport->currentY_angle_ += rotation;
      break;
    case Z:
      rotation = angle > glInterface_->currentZ_angle_
                     ? angle - glInterface_->currentZ_angle_
                     : -(glInterface_->currentZ_angle_ - angle);
      controller_->Rotate(rotation, Z);
      ui_->viewport->currentZ_angle_ += rotation;
      break;
  }
  glInterface_->update();
}

/// @brief Обработка ошибок
/// @param log - файл логирования
void MainWindow::throwError(QString log) {
  QMessageBox messageBox;
  messageBox.critical(0, "Error", log);
  messageBox.setFixedSize(500, 200);
}

///@brief Закрашивает квадратики в главном окне в соответствующие выбранные
void MainWindow::SetColors() {
  QString colorLines = QString("background: rgb(%1, %2, %3);")
                           .arg(glInterface_->rLine_ * 255)
                           .arg(glInterface_->gLine_ * 255)
                           .arg(glInterface_->bLine_ * 255);
  ui_->label_colorEdges->setStyleSheet(colorLines);
  QString colorVertices = QString("background: rgb(%1, %2, %3);")
                              .arg(glInterface_->rDot_ * 255)
                              .arg(glInterface_->gDot_ * 255)
                              .arg(glInterface_->bDot_ * 255);
  ui_->label_colorVertice->setStyleSheet(colorVertices);
  QString colorBackg = QString("background: rgb(%1, %2, %3);")
                           .arg(glInterface_->rBack_ * 255)
                           .arg(glInterface_->gBack_ * 255)
                           .arg(glInterface_->bBack_ * 255);
  ui_->label_colorBackg->setStyleSheet(colorBackg);
}

///@brief Выбор цвета граней
void MainWindow::on_pushButton_colorEdge_clicked() {
  QColor lineColor = QColorDialog::getColor(Qt::white, this);
  lineColor.getRgbF(&glInterface_->rLine_, &glInterface_->gLine_,
                    &glInterface_->bLine_);
  SetColors();
  glInterface_->update();
}

///@brief Выбор цвета вершин
void MainWindow::on_pushButton_colorVertice_clicked() {
  QColor verticeColor = QColorDialog::getColor(Qt::white, this);
  verticeColor.getRgbF(&glInterface_->rDot_, &glInterface_->gDot_,
                       &glInterface_->bDot_);
  SetColors();
  glInterface_->update();
}

///@brief Выбор цвета фона
void MainWindow::on_pushButton_colorBackg_clicked() {
  QColor backgColor = QColorDialog::getColor(Qt::white, this);
  backgColor.getRgbF(&glInterface_->rBack_, &glInterface_->gBack_,
                     &glInterface_->bBack_);
  SetColors();
  glInterface_->update();
}

///@brief Сохранение изображения выбранного формата
void MainWindow::on_pushButton_downloadImage_clicked() {
  inputhandle_->handleInput(0);
}

///@brief Обращение к команде через handle input
void MainWindow::on_pushButton_Gif_mode_1_clicked() {
  inputhandle_->handleInput(1);
}

///@brief Обращение к команде через handle input
void MainWindow::on_pushButton_Gif_mode_2_clicked() {
  inputhandle_->handleInput(2);
}

/// @brief Применяет настройки с предыдущего запуска
void MainWindow::applySettings() {
  QSettings settings("s21", "viewer");
  if (settings.contains("viewerSettings/edgeDotted")) {
    // загрузка типа проекции
    ui_->radioButton_orthogonal->setChecked(
        settings.value("viewerSettings/projection_orthogonal").toBool());

    // загрузка типа рёбер
    if (settings.value("viewerSettings/edgeDotted").toBool()) {
      ui_->radioButton_dotted->toggle();
    } else {
      ui_->radioButton_constant->toggle();
    }
    // загрузка цвета ребер
    glInterface_->rLine_ =
        settings.value("viewerSettings/edgeColor/r").toFloat();
    glInterface_->gLine_ =
        settings.value("viewerSettings/edgeColor/g").toFloat();
    glInterface_->bLine_ =
        settings.value("viewerSettings/edgeColor/b").toFloat();

    // загрузка толщины рёбер
    glInterface_->width_lines_ =
        settings.value("viewerSettings/edgeWidth").toFloat();
    ui_->edgeWidth_spinbox->setValue(
        settings.value("viewerSettings/edgeWidth").toFloat());

    // загрузка размера вершин
    glInterface_->width_dots_ =
        settings.value("viewerSettings/verticeSize").toFloat();
    ui_->verticeSize_spinbox->setValue(
        settings.value("viewerSettings/verticeSize").toFloat());

    // загрузка типа вершин
    if (settings.value("viewerSettings/noneVertice").toBool()) {
      ui_->radioButton_noneVertice->toggle();
      glInterface_->paint_dot_ = 0;
    } else if (settings.value("viewerSettings/circleVertice").toBool()) {
      ui_->radioButton_circleVertice->toggle();
      glInterface_->paint_dot_ = 1;
    } else {
      ui_->radioButton_squareVertice->toggle();
      glInterface_->paint_dot_ = 2;
    }

    // загрузка цвета вершин
    glInterface_->rDot_ =
        settings.value("viewerSettings/verticeColor/r").toFloat();
    glInterface_->gDot_ =
        settings.value("viewerSettings/verticeColor/g").toFloat();
    glInterface_->bDot_ =
        settings.value("viewerSettings/verticeColor/b").toFloat();

    // загрузка цвета фона
    glInterface_->rBack_ =
        settings.value("viewerSettings/backgroundColor/r").toFloat();
    glInterface_->gBack_ =
        settings.value("viewerSettings/backgroundColor/g").toFloat();
    glInterface_->bBack_ =
        settings.value("viewerSettings/backgroundColor/b").toFloat();

    glInterface_->update();
  }
}

///@brief Сохранение настроек перед закрытием программы
void MainWindow::saveSettings() {
  QSettings settings("s21", "viewer");
  // тип проекции
  settings.setValue("viewerSettings/projection_orthogonal",
                    ui_->radioButton_orthogonal->isChecked());
  // тип ребер
  settings.setValue("viewerSettings/edgeDotted",
                    ui_->radioButton_dotted->isChecked());
  // цвет ребер
  settings.setValue("viewerSettings/edgeColor/r", glInterface_->rLine_);
  settings.setValue("viewerSettings/edgeColor/g", glInterface_->gLine_);
  settings.setValue("viewerSettings/edgeColor/b", glInterface_->bLine_);

  // толщина рёбер
  settings.setValue("viewerSettings/edgeWidth", glInterface_->width_lines_);

  // тип вершин
  settings.setValue("viewerSettings/noneVertice",
                    ui_->radioButton_noneVertice->isChecked());
  settings.setValue("viewerSettings/circleVertice",
                    ui_->radioButton_circleVertice->isChecked());
  // размер вершин
  settings.setValue("viewerSettings/verticeSize",
                    ui_->verticeSize_spinbox->value());
  // цвет вершин
  settings.setValue("viewerSettings/verticeColor/r", glInterface_->rDot_);
  settings.setValue("viewerSettings/verticeColor/g", glInterface_->gDot_);
  settings.setValue("viewerSettings/verticeColor/b", glInterface_->bDot_);

  // цвет фона
  settings.setValue("viewerSettings/backgroundColor/r", glInterface_->rBack_);
  settings.setValue("viewerSettings/backgroundColor/g", glInterface_->gBack_);
  settings.setValue("viewerSettings/backgroundColor/b", glInterface_->bBack_);

  settings.sync();
}

///@brief Изменение масштаба спинбоксом
void MainWindow::on_scaleSpinBox_valueChanged(int arg1) {
  subject_->ValueChanged(arg1);
}

///@brief Изменение масштаба слайдером
void MainWindow::on_scaleSlider_valueChanged(int value) {
  subject_->ValueChanged(value);
}

///@brief Сброс масштаба в 100%
void MainWindow::on_scaleToDefault_clicked() { subject_->ValueChanged(100); }

///@brief Изменение масштаба слайдером
void MainWindow::on_edgeWidth_slider_valueChanged(int value) {
  ui_->edgeWidth_spinbox->setValue(value);
  glInterface_->width_lines_ = value;
  glInterface_->update();
}

///@brief Изменение масштаба через спинбокс
void MainWindow::on_edgeWidth_spinbox_valueChanged(int arg1) {
  ui_->edgeWidth_slider->setValue(arg1);
  glInterface_->width_lines_ = arg1;
  glInterface_->update();
}

/// @brief Изменение размера вершин через слайдер
/// @param value - значение нового размера точек
void MainWindow::on_verticeSize_slider_valueChanged(int value) {
  ui_->verticeSize_spinbox->setValue((double)value / 100);
  glInterface_->width_dots_ = (double)value / 100;
  glInterface_->update();
}

/// @brief Изменение размера вершин через спинбокс
/// @param value - значение нового размера точек
void MainWindow::on_verticeSize_spinbox_valueChanged(double arg1) {
  ui_->verticeSize_slider->setValue(arg1 * 100);
  glInterface_->width_dots_ = arg1;
  glInterface_->update();
}

/// @brief Изменение типа проекции на ортогональную
/// @param checked - состояние - нажато или нет
void MainWindow::on_radioButton_orthogonal_toggled(bool checked) {
  (void)checked;
  glInterface_->projectionType_ = ORTHO;
  glInterface_->update();
}

/// @brief Изменение типа проекции на перспктив
/// @param checked - состояние - нажато или нет
void MainWindow::on_radioButton_perspective_toggled(bool checked) {
  (void)checked;
  glInterface_->projectionType_ = PERSPECTIVE;
  glInterface_->update();
}

/// @brief Изменение типа линий
void MainWindow::on_radioButton_dotted_toggled(bool checked) {
  (void)checked;
  glInterface_->type_lines_ = DOTTED;
  glInterface_->update();
}

/// @brief Изменение типа линии
void MainWindow::on_radioButton_constant_toggled(bool checked) {
  (void)checked;
  glInterface_->type_lines_ = CONSTANT;
  glInterface_->update();
}

/// @brief Отключение точек
void MainWindow::on_radioButton_noneVertice_toggled(bool checked) {
  (void)checked;
  glInterface_->paint_dot_ = 0;
  glInterface_->update();
}

/// @brief Включение круглых точек
void MainWindow::on_radioButton_circleVertice_toggled(bool checked) {
  (void)checked;
  glInterface_->paint_dot_ = 1;
  glInterface_->update();
}

/// @brief Включение квадратных точек
void MainWindow::on_radioButton_squareVertice_toggled(bool checked) {
  (void)checked;
  glInterface_->paint_dot_ = 2;
  glInterface_->update();
}

///@brief Класс Subject - реализация наблюдателя
void MainWindow::Subject::ValueChanged(int value) {
  if (value != value_) {
    value_ = value;
    Notify();
    float scaleValue = (float)main_->ui_->scaleSlider->value() / 100;
    main_->controller_->Scale(main_->glInterface_->scaleM, scaleValue);
    main_->glInterface_->scaleM = scaleValue;
    main_->glInterface_->update();
  }
}

///@brief Оповещение - изменение положения ползунков
void MainWindow::Subject::Notify() {
  if (main_->ui_->scaleSlider->value() != value_ &&
      main_->ui_->scaleSpinBox->value() != value_) {
    main_->ui_->scaleSlider->setValue(value_);
    main_->ui_->scaleSpinBox->setValue(value_);
  } else if (main_->ui_->scaleSpinBox->value() != value_) {
    main_->ui_->scaleSpinBox->setValue(value_);
  } else {
    main_->ui_->scaleSlider->setValue(value_);
  }
}

}  // namespace s21
