#include "command.h"

#include "ui_mainwindow.h"

namespace s21 {

///@brief Сохранение .gif в заданном разрешении
void GifModeCommand::makeGif() {
  QGifImage gif;
  if (frameCount_ == 50) {
    tmr_->stop();
    disconnect(tmr_);
    delete (tmr_);
    frameCount_ = 0;
    for (int i = 0; i < 50; gif.addFrame(images_[i++], 100)) {
    }
    QString str =
        QFileDialog::getSaveFileName(0, "Сохранить файл как", "", "*.gif");
    gif.save(str);
  } else {
    if (gifMode_ == 1) {
      images_[frameCount_++] =
          main_wind_->glInterface_->grabFramebuffer().scaled(
              640, 480, Qt::IgnoreAspectRatio);
    } else if (gifMode_ == 2) {
      images_[frameCount_++] = main_wind_->glInterface_->grabFramebuffer();
    }
  }
}

void GifModeCommand::execute(int gif_mode) {  // gif mode 1 and 2
  frameCount_ = 0;
  gifMode_ = gif_mode;
  tmr_ = new QTimer();
  connect(tmr_, SIGNAL(timeout()), this, SLOT(makeGif()));
  tmr_->start(100);
}

void InputHandler::handleInput(int input) {
  if (input == 0)
    buttonImage_->execute();
  else if (input == 1)
    buttonGif_->execute(1);
  else if (input == 2)
    buttonGif_->execute(2);
}

///@brief Сохранение скриншота в формате jpg and bmp
void DownloadImageCommand::execute() {
  QImage screenShot = main_wind_->glInterface_->grabFramebuffer();
  QString format =
      (main_wind_->ui_->radioButton_jpgFormat->isChecked()) ? "*.jpg" : "*.bmp";
  QString str =
      QFileDialog::getSaveFileName(0, "Сохранить файл как", "", format);
  screenShot.save(str, format.remove(0, 2).toUpper().toLocal8Bit());
}
}  // namespace s21
