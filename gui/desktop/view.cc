/**
 * @file view.cc
 * @brief Contains definition for View class methods.
 */

#include "view.h"

#include "./ui_view.h"

View::View(QWidget *parent) : QMainWindow(parent), ui(new Ui::View) {
  ui->setupUi(this);

  this->setWindowTitle("BrickGame");

  ui->stackedWidget->setCurrentIndex(0);

  connect(ui->snakeFieldWidget, &SnakeField::stateUpdated, this,
          [this](const s21::Snake_GameInfo_t &gi) {
            ui->infoLabel->setText(QString("<p align='center'>INFO:</p>"
                                           "<br>"
                                           "High Score:<br>&nbsp;&nbsp;%1<br>"
                                           "Score:<br>&nbsp;&nbsp;%2<br>"
                                           "Level:<br>&nbsp;&nbsp;%3")
                                       .arg(gi.high_score)
                                       .arg(gi.score)
                                       .arg(gi.level));
          });

  connect(
      ui->tetrisFieldWidget, &TetrisField::stateUpdated, this,
      [this](const GameInfo_t &t_gi) {
        ui->tetrisInfoLabel->setText(QString("<p align='center'>INFO:</p>"
                                             "<br>"
                                             "High Score:<br>&nbsp;&nbsp;%1<br>"
                                             "Score:<br>&nbsp;&nbsp;%2<br>"
                                             "Level:<br>&nbsp;&nbsp;%3<br>"
                                             "Next Figure:")
                                         .arg(t_gi.high_score)
                                         .arg(t_gi.score)
                                         .arg(t_gi.level));

        ui->nextFigureWidget->update();
      });

  connect(ui->snakeFieldWidget, &SnakeField::exitRequested, this,
          &QMainWindow::close);
  connect(ui->tetrisFieldWidget, &TetrisField::exitRequested, this,
          &QMainWindow::close);
}

View::~View() { delete ui; }

void View::on_listWidget_itemClicked(QListWidgetItem *item) {
  if (item->text() == "Tetris") {
    ui->stackedWidget->setCurrentIndex(2);
    ui->tetrisFieldWidget->setFocus();
  } else if (item->text() == "Snake") {
    ui->stackedWidget->setCurrentIndex(1);
    ui->snakeFieldWidget->setFocus();
  }

  auto controlsLabel = ui->stackedWidget->findChild<QLabel *>("controlsLabel");
  controlsLabel->setText(
      "<p align='center'>CONTROLS:</p>"
      "<br>"
      "Enter - start<br>"
      "q - quit<br>"
      "p - pause<br>"
      "<br>"
      "&lt; - left<br>"
      "&gt; - right<br>"
      "a - speed");

  auto tetrisControlsLabel =
      ui->stackedWidget->findChild<QLabel *>("tetrisControlsLabel");
  tetrisControlsLabel->setText(
      "<p align='center'>CONTROLS:</p>"
      "<br>"
      "Enter - start<br>"
      "q - quit<br>"
      "p - pause<br>"
      "<br>"
      "&lt; - left<br>"
      "&gt; - right<br>"
      "v - right<br>"
      "r - rotate");

  auto frame = ui->stackedWidget->findChild<QFrame *>("frame");
  frame->setFrameStyle(QFrame::Box);
  auto tetrisFrame = ui->stackedWidget->findChild<QFrame *>("tetrisFrame");
  tetrisFrame->setFrameStyle(QFrame::Box);
}
