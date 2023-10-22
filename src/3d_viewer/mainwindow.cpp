#include "mainwindow.h"
#include "myopenglwidgegt.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  save_settings = QApplication::applicationDirPath() + "/save_settings.conf";
  load_Settings();


}

MainWindow::~MainWindow() {

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(
      this, "Saving settings",
      "Do you want to save the settings before closing the application?",
      QMessageBox::Save | QMessageBox::Discard);
  if (reply == QMessageBox::Save) {
    save_Settings();
  } else {
    save_Settings_base();
}
  delete ui;
}


void MainWindow::on_pushButton_chooseFile_clicked() {
  QString file_path =
      QFileDialog::getOpenFileName(this, "Select file", QDir::currentPath(),
                                   "All Files (*.*);; Obj files (*.obj);");

  QStringList list_of_name = file_path.split(u'/');
  QString file_name = list_of_name[list_of_name.size() - 1];
  ui->label_textFile->setText(file_name);

  data_t matrix = {0};
  matrix.matrix_3d.cols = 3;
  matrix.matrix_3d.rows = 0;
  matrix.count_of_facets = 0;
  matrix.count_of_vertexrs = 0;
  std::string math_exp_str = file_path.toStdString();
  const char *math_expression = math_exp_str.c_str();

  if (open_obj_file(math_expression, &matrix)) {
    ui->horizontalSlider_scale->setValue(50);
    ui->horizontalSlider_x_value->setValue(0);
    ui->horizontalSlider_y_value->setValue(0);
    ui->horizontalSlider_z_value->setValue(0);
    ui->horizontalSlider_x_translate->setValue(0);
    ui->horizontalSlider_y_translate->setValue(0);
    ui->horizontalSlider_z_translate->setValue(0);

    old_position_x = 0;
    old_position_y = 0;
    old_position_z = 0;
    old_scale = 0.5;

    if (file_open)
      clear_memory(&(ui->openGLWidget->matrix));
    ui->openGLWidget->matrix = matrix;
    ui->openGLWidget->status_paint = true;
    file_open = true;
    ui->lcdNumber_vertices->display(
        static_cast<int>(ui->openGLWidget->matrix.count_of_vertexrs));
    ui->lcdNumber_edges->display(
        static_cast<int>(ui->openGLWidget->matrix.count_of_facets));

    update();
  } else
    QMessageBox::critical(this, "Warning", "Something went wrong");
}

void MainWindow::on_horizontalSlider_x_value_valueChanged(int value) {
  double tmp = value;
  value -= old_position_x;
  // else old_position+=value;
  // if (file_open) {
  move_figure_x_y_z(&(ui->openGLWidget->matrix), value * 0.01, X);
  ui->openGLWidget->update();
  old_position_x = tmp;
  //    } else {
  //    QMessageBox::critical(this,"Warning","File is not load");
  //    }
}

void MainWindow::on_horizontalSlider_y_value_valueChanged(int value) {
  double tmp = value;
  value -= old_position_y;
  // else old_position+=value;
  //    if (file_open) {
  move_figure_x_y_z(&(ui->openGLWidget->matrix),
                    static_cast<double>(value) * 0.01, Y);
  ui->openGLWidget->update();
  old_position_y = tmp;

  //    } else {
  //    QMessageBox::critical(this,"Warning","File is not load");
  //    }
}

void MainWindow::on_horizontalSlider_z_value_valueChanged(int value) {
  double tmp = value;
  value -= old_position_z;
  // else old_position+=value;
  //    if (file_open) {
  move_figure_x_y_z(&(ui->openGLWidget->matrix), value * 0.01, Z);
  ui->openGLWidget->update();
  old_position_z = tmp;

  //    } else {
  //    QMessageBox::critical(this,"Warning","File is not load");
  //    }
}

void MainWindow::on_horizontalSlider_scale_valueChanged(int value) {
  double tmp = value;
  tmp /= 100;
  double tmp_value = value;
  tmp_value /= 100;
  tmp_value /= old_scale;
  // else old_position+=value;
  //    if (file_open) {
  increase_reduction_figure_a(&(ui->openGLWidget->matrix), tmp_value);
  ui->openGLWidget->update();
  old_scale = tmp;
  ui->label_scale->setText("Scale: " + QString::number(value) + "%");
  //    } else {
  //    QMessageBox::critical(this,"Warning","File is not load");
  //    }
}

void MainWindow::on_horizontalSlider_x_translate_valueChanged(int value) {
  //    if (file_open) {
  rotation_by_x_y_z(&(ui->openGLWidget->matrix),
                    static_cast<double>(value) * 0.01, X);
  ui->openGLWidget->update();
  //    } else {
  //         QMessageBox::critical(this,"Warning","File is not load");
  //    }
}

void MainWindow::on_horizontalSlider_y_translate_valueChanged(int value) {
  //    if (file_open) {
  rotation_by_x_y_z(&(ui->openGLWidget->matrix),
                    static_cast<double>(value) * 0.01, Y);
  ui->openGLWidget->update();
  //    } else {
  //         QMessageBox::critical(this,"Warning","File is not load");
  //    }
}

void MainWindow::on_horizontalSlider_z_translate_valueChanged(int value) {
  //    if (file_open) {
  rotation_by_x_y_z(&(ui->openGLWidget->matrix),
                    static_cast<double>(value) * 0.01, Z);
  ui->openGLWidget->update();
  //    } else {
  //         QMessageBox::critical(this,"Warning","File is not load");
  //    }
}

void MainWindow::on_CentralProjectionSelected() {
  //    if (file_open) {
  ui->openGLWidget->setProjectionType(Central);
  //    } else {
  //         QMessageBox::critical(this, "Warning", "File is not loaded");
  //    }
}

void MainWindow::on_ParallelProjectionSelected() {
  //    if (file_open) {
  ui->openGLWidget->setProjectionType(Parallel);
  //    } else {
  //         QMessageBox::critical(this, "Warning", "File is not loaded");
  //    }
}

void MainWindow::on_peaks_color_clicked() {
  QColor chosenColor =
      QColorDialog::getColor(Qt::white, this, "Выберите цвет фигуры");
  if (chosenColor.isValid()) {
    float chosen_red_point = chosenColor.redF();
    float chosen_green_point = chosenColor.greenF();
    float chosen_blue_point = chosenColor.blueF();
    ui->openGLWidget->setPointColor(chosen_red_point, chosen_green_point,
                                    chosen_blue_point);
  }
}

void MainWindow::on_size_point_valueChanged(int value) {
  ui->openGLWidget->setsizepoint(value);
  ui->openGLWidget->update();
}

void MainWindow::on_background_color_clicked() {
  //    if (file_open) {
  QColor chosenColor =
      QColorDialog::getColor(Qt::white, this, "Выберите цвет текста");

  if (chosenColor.isValid()) {

    float chosen_red_background = chosenColor.redF();
    float chosen_green_background = chosenColor.greenF();
    float chosen_blue_background = chosenColor.blueF();
    float chosen_alpha_background = chosenColor.alphaF();
    ui->openGLWidget->setBackgroundColor(
        chosen_red_background, chosen_green_background, chosen_blue_background,
        chosen_alpha_background);
  }
  /*}else {
     QMessageBox::critical(this, "Предупреждение", "Файл не загружен");
 }*/
}
void MainWindow::on_gif_clicked() {
  if (file_open) {
    QGifImage gifImage(QSize(640, 480));
    gifImage.setDefaultDelay(100);
    gifImage.setLoopCount(0);

    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < 5 * 1000) {
      QImage frameImage = ui->openGLWidget->grabFramebuffer();
      gifImage.addFrame(frameImage);
    }

    QString filter = "GIF (*.gif)";
    QString FileName =
        QFileDialog::getSaveFileName(this, "Save Image", "", filter);

    if (!FileName.isEmpty()) {
      QFile file(FileName);
      if (file.open(QIODevice::WriteOnly)) {
        gifImage.save(&file);
        file.close();
        QMessageBox::information(this, "Success", "Image saved successfully");
      } else {
        QMessageBox::critical(this, "Error", "Could not save GIF animation");
      }
    }
  } else {
    QMessageBox::critical(this, "Warning", "File is not load");
  }
}

void MainWindow::on_screen_clicked() {
  if (file_open) {
    QImage GraphicImage = ui->openGLWidget->grabFramebuffer();

    QString filter = "Images (*.jpg *.bmp);;JPEG (*.jpg);;BMP (*.bmp)";
    QString FileName =
        QFileDialog::getSaveFileName(this, "Save Image", "", filter);

    if (!FileName.isEmpty()) {
      if (GraphicImage.save(FileName)) {
        QMessageBox::information(this, "Success", "Image saved successfully");
      } else {
        QMessageBox::critical(this, "Error", "Could not save image");
      }
    }
  } else {
    QMessageBox::critical(this, "Warning", "File is not load");
  }
}

void MainWindow::on_color_clicked() {
  QColor chosenColor =
      QColorDialog::getColor(Qt::white, this, "Выберите цвет фигуры");
  if (chosenColor.isValid()) {
    float chosen_red = chosenColor.redF();
    float chosen_green = chosenColor.greenF();
    float chosen_blue = chosenColor.blueF();
    ui->openGLWidget->setFigureColor(chosen_red, chosen_green, chosen_blue);
  }
}

void MainWindow::on_without_clicked() {
  int type_without = 3;
  ui->openGLWidget->setTypePoint(type_without);
}

void MainWindow::on_circle_clicked() {
  int type_circle = 1;
  ui->openGLWidget->setTypePoint(type_circle);
}

void MainWindow::on_square_clicked() {
  int type_square = 2;
  ui->openGLWidget->setTypePoint(type_square);
}

void MainWindow::load_Settings() {

  QSettings settings(save_settings, QSettings::IniFormat);


if (settings.allKeys().isEmpty()) {
    save_Settings_base();
}
  ui->openGLWidget->type_point = settings.value("type_point").toInt();
  ui->openGLWidget->size_point = settings.value("size_point").toInt();
  ui->openGLWidget->edgeThickness = settings.value("size_line").toInt();
  ui->openGLWidget->useDashedLine = settings.value("useDashedLine").toBool();
  ui->openGLWidget->currentProjectionType = static_cast<ProjectionType>(
      settings.value("currentProjectionType").toInt());
  ui->openGLWidget->red_background =
      settings.value("colorBackground_red").toFloat();
  ui->openGLWidget->green_background =
      settings.value("colorBackground_green").toFloat();
  ui->openGLWidget->blue_background =
      settings.value("colorBackground_blue").toFloat();
  ui->openGLWidget->alpha_background =
      settings.value("colorBackground_alpha").toFloat();

  ui->openGLWidget->red = settings.value("colorredFigure").toFloat();
  ui->openGLWidget->green = settings.value("colorgreenFigure").toFloat();
  ui->openGLWidget->blue = settings.value("colorblueFigure").toFloat();

  ui->openGLWidget->red_point = settings.value("colorPoint_red").toFloat();
  ui->openGLWidget->green_point = settings.value("colorPoint_green").toFloat();
  ui->openGLWidget->blue_point = settings.value("colorPoint_blue").toFloat();
}

void MainWindow::save_Settings() {
  QSettings settings(save_settings, QSettings::IniFormat);
  settings.setValue("type_point", ui->openGLWidget->type_point);
  settings.setValue("size_point", ui->openGLWidget->size_point);
  settings.setValue("size_line", ui->openGLWidget->edgeThickness);
  settings.setValue("useDashedLine", ui->openGLWidget->useDashedLine);
  settings.setValue("useDashedLine", ui->openGLWidget->currentProjectionType);
  float redBackground = ui->openGLWidget->red_background;
  float greenBackground = ui->openGLWidget->green_background;
  float blueBackground = ui->openGLWidget->blue_background;
  float alphaBackground = ui->openGLWidget->alpha_background;

  settings.setValue(
      "colorBackground_red",
      QLocale::c().toString(static_cast<double>(redBackground), 'f', 6));
  settings.setValue(
      "colorBackground_green",
      QLocale::c().toString(static_cast<double>(greenBackground), 'f', 6));
  settings.setValue(
      "colorBackground_blue",
      QLocale::c().toString(static_cast<double>(blueBackground), 'f', 6));
  settings.setValue(
      "colorBackground_alpha",
      QLocale::c().toString(static_cast<double>(alphaBackground), 'f', 6));

  float redFigure = ui->openGLWidget->red;
  float greenFigure = ui->openGLWidget->green;
  float blueFigure = ui->openGLWidget->blue;

  settings.setValue(
      "colorredFigure",
      QLocale::c().toString(static_cast<double>(redFigure), 'f', 6));
  settings.setValue(
      "colorgreenFigure",
      QLocale::c().toString(static_cast<double>(greenFigure), 'f', 6));
  settings.setValue(
      "colorblueFigure",
      QLocale::c().toString(static_cast<double>(blueFigure), 'f', 6));

  float redPoint = ui->openGLWidget->red_point;
  float greenPoint = ui->openGLWidget->green_point;
  float bluePoint = ui->openGLWidget->blue_point;

  settings.setValue(
      "colorPoint_red",
      QLocale::c().toString(static_cast<double>(redPoint), 'f', 6));
  settings.setValue(
      "colorPoint_green",
      QLocale::c().toString(static_cast<double>(greenPoint), 'f', 6));
  settings.setValue(
      "colorPoint_blue",
      QLocale::c().toString(static_cast<double>(bluePoint), 'f', 6));
}

void MainWindow::save_Settings_base() {
  QSettings settings(save_settings, QSettings::IniFormat);
  settings.setValue("colorBackground_red", 0.0f);
  settings.setValue("colorBackground_green", 0.0f);
  settings.setValue("colorBackground_blue", 0.0f);
  settings.setValue("colorBackground_alpha", 1.0f);
  settings.setValue("colorredFigure", 1.0);
  settings.setValue("colorgreenFigure", 1.0);
  settings.setValue("colorblueFigure", 1.0);
  settings.setValue("colorPoint_red", 1.0);
  settings.setValue("colorPoint_green", 1.0);
  settings.setValue("colorPoint_blue", 1.0);
  settings.setValue("type_point", 3);
}

void MainWindow::on_checkBox_dashedLine_stateChanged(int state) {
  bool useDashedLine = (state == Qt::Checked);
  ui->openGLWidget->setDashedLine(useDashedLine);
}

void MainWindow::on_slider_edgeThickness_valueChanged(int value) {
  ui->openGLWidget->setEdgeThickness(value);
  ui->openGLWidget->update();
}

void MainWindow::on_radioButtonParal_clicked() {
  int value = 1;
  ui->openGLWidget->setProjectionType(value);
}

void MainWindow::on_radioButtonCentral_clicked() {
  int value = 2;
  ui->openGLWidget->setProjectionType(value);
}

void MainWindow::on_radioButtonNON_clicked() {
  int value = 3;
  ui->openGLWidget->setProjectionType(value);
}
