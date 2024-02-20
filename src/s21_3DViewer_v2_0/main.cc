#include <QApplication>

#include "View/mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  s21::Model* object = s21::Model::GetInstance();
  s21::Controller control(object);
  s21::MainWindow w(&control);
  w.setWindowTitle("3DViewer v2.0");
  w.show();
  return a.exec();
}
