#ifndef S21_3DVIEWER_V2_0_MAINWINDOW_H
#define S21_3DVIEWER_V2_0_MAINWINDOW_H

#include <qgifimage.h>

#include <QBuffer>
#include <QButtonGroup>
#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QTimer>

#include "Controller/controller.h"
#include "Model/model.h"
#include "View/command.h"
#include "View/glwidget.h"

//!  Основной класс приложения
/*!
  Методы класса позволяют менять настройки модели, саму модель и реализовывать
  другие действия.
*/

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class InputHandler;
// class Subject;

class MainWindow : public QMainWindow {
  Q_OBJECT
  friend class GifModeCommand;
  friend class DownloadImageCommand;
  friend class Subject;

 public:
  MainWindow(s21::Controller *control, QWidget *parent = nullptr);
  ~MainWindow();
  void SetColors();  //!< установка базового цвета

 private slots:
  void on_open_file_clicked();  //!< метод загрузки данных из файла
  void on_MainWindow_destroyed();

  // Цвета
  void on_pushButton_colorEdge_clicked();
  void on_pushButton_colorVertice_clicked();
  void on_pushButton_colorBackg_clicked();

  // движение модели
  void on_moveXInput_valueChanged(double arg1);
  void on_moveYInput_valueChanged(double arg1);
  void on_moveZInput_valueChanged(double arg1);

  // вращение модели + изменение значение угла поворота с помощью слайдера
  void rotateModel(axes axis, float angle);
  void on_rotateXInput_valueChanged(double arg1);
  void on_rotateYInput_valueChanged(double arg1);
  void on_rotateZInput_valueChanged(double arg1);
  void on_rotationX_slider_valueChanged(int value);
  void on_rotationY_slider_valueChanged(int value);
  void on_rotationZ_slider_valueChanged(int value);

  // gif
  void on_pushButton_Gif_mode_1_clicked();
  void on_pushButton_Gif_mode_2_clicked();

  void
  on_pushButton_downloadImage_clicked();  // рендеринг и скачивание фотографии

  // масштаб
  void on_scaleSpinBox_valueChanged(int arg1);
  void on_scaleSlider_valueChanged(int value);
  void on_scaleToDefault_clicked();

  // толщина рёбер
  void on_edgeWidth_slider_valueChanged(int value);
  void on_edgeWidth_spinbox_valueChanged(int arg1);

  // размер вершин
  void on_verticeSize_spinbox_valueChanged(double arg1);
  void on_verticeSize_slider_valueChanged(int value);

  // радиобаттоны
  void on_radioButton_orthogonal_toggled(bool checked);
  void on_radioButton_perspective_toggled(bool checked);
  void on_radioButton_dotted_toggled(bool checked);
  void on_radioButton_constant_toggled(bool checked);
  void on_radioButton_noneVertice_toggled(bool checked);
  void on_radioButton_circleVertice_toggled(bool checked);
  void on_radioButton_squareVertice_toggled(bool checked);

 private:
  class Subject {
   public:
    Subject(MainWindow *main) : main_(main){};
    ~Subject() {}
    void ValueChanged(int value);

   private:
    void Notify();

    int value_ = 1;
    MainWindow *main_;
  };

  Ui::MainWindow *ui_;
  GLWidget *glInterface_;
  s21::Controller *controller_;
  InputHandler *inputhandle_;
  Subject *subject_;

  void throwError(QString log);  //!< создание уведомления об ошибке
  void resetMovement();  //!< сброс настроек перемещения модели
  void saveSettings();  //!< сохранение настроек (применяется после закрытия
  //!< программы)
  void applySettings();  //!< применение настроек (вызывается после открытия
  //!< программы)
  double moveX_ = 0;
  double moveY_ = 0;
  double moveZ_ = 0;
};

}  // namespace s21

#endif  // S21_3DVIEWER_V2_0_MAINWINDOW_H
