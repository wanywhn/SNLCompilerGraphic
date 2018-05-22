#include "lexscene.h"
#include "ll1_parse.h"
#include "mainwindow.h"
#include "parse.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsWidget>
#include <QGroupBox>
#include <QtWidgets/QWidgetAction>
#include <QtGui/QSyntaxHighlighter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  init_ui();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::init_ui() {

    this->setMinimumWidth(800);
    this->setMinimumHeight(600);
    auto leftsplitter=new QSplitter(Qt::Horizontal);

  editer_left = new QPlainTextEdit(leftsplitter);
  tab_center_widget = new QTabWidget(leftsplitter);
  // TODO Add tabview
  lexscene=new LexScene();
  parseScene=new ParseScene();

  auto view=new QGraphicsView(tab_center_widget);
  view->setScene(lexscene);
  pview=new QGraphicsView(tab_center_widget);
  pview->setScene(parseScene);
  tab_center_widget->addTab(view,QString(tr("LEX")));
  tab_center_widget->addTab(pview,QString(tr("Parse")));



  rightsplitter = new QSplitter(Qt::Vertical,leftsplitter);

  auto cclabel=new QLabel(rightsplitter);
  cclabel->setText(tr("CURRENT CHAR:"));

  label_current_char = new QLabel(rightsplitter);
  label_current_char->setText("EMPTY");

  auto cllabel=new QLabel(rightsplitter);
  cllabel->setText(tr("CURRENT LINE:"));

  label_current_line = new QLabel(rightsplitter);
  label_current_line->setText("EMPTY");

  listwidget_token = new QListWidget(rightsplitter);
  listwidget_token->setMinimumWidth(230);

    this->setCentralWidget(leftsplitter);

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

  auto menu_lex=this->menuBar()->addMenu("LEX");

  menu_lex->addSeparator();

  auto speed_slider=new QSlider(Qt::Horizontal,menu_lex);
  speed_slider->setMaximum(100);
  speed_slider->setMinimum(1);
  speed_slider->setValue(100);
  connect(speed_slider,&QSlider::valueChanged,[this](int value){
    lex->set_speed(value);
  });

  auto speed_action=new QWidgetAction(menu_lex);
  speed_action->setDefaultWidget(speed_slider);

  menu_lex->addAction(speed_action);

  auto action_lex=menu_lex->addAction("Lex");
  connect(action_lex, &QAction::triggered, [this]() {
  lex = Lex::getInstance(filename);
  connect(lex, &Lex::charget, this, &MainWindow::char_changed);
  connect(lex, &Lex::idbuff_changed, this, &MainWindow::idbuff_changed);
  connect(lex,&Lex::token_get,this,&MainWindow::token_get);
  connect(lex,&Lex::go_path,lexscene,&LexScene::show_path);
  lex->start();

  });


  auto action_parse=this->menuBar()->addMenu(tr("Parse"));
  auto re=action_parse->addAction(tr("re"));
  connect(re,&QAction::triggered,this,&MainWindow::re_parse);
  auto LL1=action_parse->addAction(tr("LL1"));
  connect(LL1,&QAction::triggered,this,&MainWindow::ll1_parse);
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
    QString str("Line:%1\tLex:%2\t\tSem:%3");
    QString item=str.arg(token->getLine()).arg(token->getLexName()).arg(token->getSem());
    listwidget_token->addItem(item);

}

void MainWindow::re_parse()
{
     auto re=Parse::getInstance(lex->getTokenList());
     connect(re.data(),&Parse::parse_success,[this,re](){
         parseScene->show_parsetree(QSharedPointer<TreeNode>(re->get_parsetree_head()),"递归下降");
     });
     re->start();

}

void MainWindow::ll1_parse()
{
    auto ll1=LL1_parse::getInstance(lex->getTokenList());
    connect(ll1.data(),&LL1_parse::parse_success,[this,ll1](){
        parseScene->show_parsetree(QSharedPointer<TreeNode>(ll1->get_parsetree_head()),"LL1");
    });
    ll1->start();

}
