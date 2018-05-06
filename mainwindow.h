#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lex.h"

#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    void init_ui();

private:
    Ui::MainWindow *ui;

    QSplitter *splitter;
    QPlainTextEdit *editer_left;
    QTabWidget *tab_center_widget;
    QVBoxLayout *layout_right;
    QHBoxLayout *layout_left;
    QHBoxLayout *layout;
    QLabel *label_current_char;
    QLabel *label_current_line;
    QListWidget *listwidget_token;

    QString filename;
    QTextStream *ins;
    Lex *lex;
};

#endif // MAINWINDOW_H
