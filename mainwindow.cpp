#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init_ui();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_ui()
{
    splitter=new QSplitter();

    layout=new QHBoxLayout();


    editer_left=new QPlainTextEdit(splitter);
    tab_center_widget=new QTabWidget(splitter);
    //TODO Add tabview

    layout->addWidget(splitter);

    layout_right=new QVBoxLayout();
    label_current_char=new QLabel(this);
    label_current_char->setText("test");
    label_current_line=new QLabel(this);
    label_current_line->setText("xx");
    listwidget_token=new QListWidget(this);
    listwidget_token->setMaximumWidth(200);
    layout_right->addWidget(label_current_char);
    layout_right->addWidget(label_current_line);
    layout_right->addWidget(listwidget_token);

    layout->addLayout(layout_right);

    this->centralWidget()->setLayout(layout);

    auto openfile=new QAction(QString(tr("打开源代码")),this->menuBar());
    auto filemenu=this->menuBar()->addMenu(tr("&File"));
    filemenu->addAction(openfile);
    connect(openfile,&QAction::triggered,[=](){
        filename=QFileDialog::getOpenFileName(this,tr("请选择源代码文件"));
        auto file=new QFile(filename,this);
        file->open(QIODevice::ReadWrite);
        ins->setDevice(file);
        editer_left->document()->setPlainText(ins->readAll());

    });
    auto exit=filemenu->addAction(tr("Exit"));
    connect(exit,&QAction::triggered,[](){qApp->exit();});


    auto action_lex=this->menuBar()->addAction(tr("LEX"));
    lex=Lex::getInstance();
    connect(action_lex,&QAction::triggered,[this](){

        lex->setFileName(filename);

    });

    ins=new QTextStream();
}
