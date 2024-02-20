#ifndef S21_3DVIEWER_V2_0_COMMAND_H
#define S21_3DVIEWER_V2_0_COMMAND_H

#include <QMainWindow>

#include "View/glwidget.h"
#include "View/mainwindow.h"

namespace s21 {

class MainWindow;

/*------Command----*/
class Command : public QMainWindow {
 public:
  virtual ~Command() {}
  virtual void execute() {}

 protected:
  MainWindow *main_wind_;
};

/*------End command*/

class DownloadImageCommand : public Command {
 public:
  DownloadImageCommand(MainWindow *main_wind) { main_wind_ = main_wind; }
  void execute();
};

class GifModeCommand : public Command {
  Q_OBJECT
 public:
  GifModeCommand(MainWindow *main_wind) { main_wind_ = main_wind; }

  void execute(int gif_mode);

 private slots:
  void makeGif();

 private:
  int frameCount_;
  int gifMode_ = 1;
  QTimer *tmr_;
  QImage images_[50];
};

class InputHandler {
 public:
  InputHandler(MainWindow *main_wind) {
    buttonGif_ = new GifModeCommand(main_wind);
    buttonImage_ = new DownloadImageCommand(main_wind);
  }

  ~InputHandler() {
    delete buttonGif_;
    delete buttonImage_;
  }

  void handleInput(int input);

 private:
  GifModeCommand *buttonGif_;
  DownloadImageCommand *buttonImage_;
};

}  // namespace s21

#endif  // S21_3DVIEWER_V2_0_COMMAND_H
