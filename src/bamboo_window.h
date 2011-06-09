#ifndef GOWINDOW_H
#define GOWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>

#include "bamboo_widget.h"
#include "bamboo_gamerecord.h"

class BambooWindow : public QMainWindow
{
    Q_OBJECT

public:
    BambooWindow();
    ~BambooWindow();
    void addMove(GoMoveState color, unsigned short x, unsigned short y);
    void addMove(unsigned short x, unsigned short y);
    GoMoveState getNextMove() const;
    void newGame(const unsigned short size);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void about();
    void newGameFromMenu();

private:
    void createActions();
    void createMenus();
    void newGame();

    BambooWidget *boardWidget;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *newAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    BambooGamerecord *gameRecord;
};

#endif
