#include <iostream>

#include <QtGui>
#include <QString>
#include <QByteArray>

#include "bamboo_window.h"

BambooWindow::BambooWindow()
{
    boardWidget = new BambooWidget;

    newGame(13);

    setWindowTitle(tr("Bamboo"));
    setCentralWidget(boardWidget);

    createActions();
    createMenus();
}

BambooWindow::~BambooWindow()
{
    delete gameRecord;
    delete boardWidget;

    delete fileMenu;
    delete helpMenu;
    delete exitAct;
    delete aboutAct;
    delete aboutQtAct;
}

void BambooWindow::newGame(const unsigned short size)
{
    std::cout << "-- New Game --" << std::endl;
    gameRecord = new BambooGamerecord(size);
    boardWidget->setMatrix( gameRecord->getMatrix() );
    boardWidget->update();
}

void BambooWindow::newGameFromMenu()
{
    delete gameRecord;
    newGame(13);
}

void BambooWindow::addMove(GoMoveState color, unsigned short x, unsigned short y)
{
    gameRecord->addMove(color, x, y);
    boardWidget->setMatrix( gameRecord->getMatrix() );
}

void BambooWindow::addMove(unsigned short x, unsigned short y)
{
    gameRecord->addMove(x, y);
    boardWidget->setMatrix( gameRecord->getMatrix() );
}

GoMoveState BambooWindow::getNextMove() const
{
    return gameRecord->getNextMove();
}

void BambooWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void BambooWindow::about()
{
    QByteArray str("<p><b>Bamboo</b> is a graphical interface for Go (Japanese: 碁), "
                   "Baduk (Korean: 바둑), or Wéiqí (traditional Chinese: 圍棋, "
                   "simplified: 围棋) written in C++ using <a href='http://qt.nokia.com'>Qt</a> "
                   "bindings.</p>");
    QMessageBox::about(this, tr("About Application"), QString::fromUtf8(str));
}

void BambooWindow::createActions()
{
    newAct = new QAction(tr("&New Game"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("New game"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newGameFromMenu()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    
    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void BambooWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Go"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
