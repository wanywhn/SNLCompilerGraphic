#include "lexview.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsWidget>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  init_ui();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::init_ui() {
  splitter = new QSplitter();

  layout = new QHBoxLayout();

  editer_left = new QPlainTextEdit(splitter);
  tab_center_widget = new QTabWidget(splitter);
  // TODO Add tabview
  lexView=new LexView();
  parseView=new QGraphicsView();

  tab_center_widget->addTab(lexView,QString(tr("LEX")));
  tab_center_widget->addTab(parseView,QString(tr("Parse")));


  layout->addWidget(splitter);

  layout_right = new QVBoxLayout();

  auto cclabel=new QLabel(this);
  cclabel->setText(tr("CURRENT CHAR:"));

  label_current_char = new QLabel(this);
  label_current_char->setText("EMPTY");

  auto cllabel=new QLabel(this);
  cllabel->setText(tr("CURRENT LINE:"));

  label_current_line = new QLabel(this);
  label_current_line->setText("EMPTY");

  listwidget_token = new QListWidget(this);
  listwidget_token->setMaximumWidth(200);
  layout_right->addWidget(cclabel);
  layout_right->addWidget(label_current_char);
  layout_right->addWidget(cllabel);
  layout_right->addWidget(label_current_line);
  layout_right->addWidget(listwidget_token);

  layout->addLayout(layout_right);

  this->centralWidget()->setLayout(layout);

  auto openfile = new QAction(QString(tr("打开源代码")), this->menuBar());
  auto filemenu = this->menuBar()->addMenu(tr("&File"));
  filemenu->addAction(openfile);
  connect(openfile, &QAction::triggered, [=]() {
    filename = QFileDialog::getOpenFileName(this, tr("请选择源代码文件"));
    auto file = new QFile(filename, this);
    file->open(QIODevice::ReadWrite);
    ins->setDevice(file);
    editer_left->document()->setPlainText(ins->readAll());

  });
  auto exit = filemenu->addAction(tr("Exit"));
  connect(exit, &QAction::triggered, []() { qApp->exit(); });

  auto action_lex = this->menuBar()->addAction(tr("LEX"));
  connect(action_lex, &QAction::triggered, [this]() {
  lex = Lex::getInstance(filename);
  connect(lex, &Lex::charget, this, &MainWindow::char_changed);
  connect(lex, &::Lex::idbuff_changed, this, &MainWindow::idbuff_changed);
  connect(lex,&Lex::token_get,this,&MainWindow::token_get);
  lex->start();

  });

  auto action_parse=this->menuBar()->addMenu(tr("Parse"));
  auto re=action_parse->addAction(tr("re"));
  connect(re,&QAction::triggered,this,&MainWindow::re_parse);
  auto LL1=action_parse->addAction(tr("LL1"));
  connect(LL1,&QAction::triggered,this,&MainWindow::LL1_parse);
  ins = new QTextStream();
}

void MainWindow::char_changed(char c) {
  label_current_char->setText("\'"+QString(c)+"\'");
}

void MainWindow::idbuff_changed(QString str) {
    label_current_line->setText("\""+str+"\"");
}

void MainWindow::token_get(Token *token)
{
    QString str("Line:%1\tLex:%2\tSem:%3");
    QString item=str.arg(token->getLine()).arg(token->getLexName()).arg(token->getSem());
    listwidget_token->addItem(item);

}

void MainWindow::re_parse()
{

}

void MainWindow::LL1_parse()
{

}
