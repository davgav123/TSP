#ifndef WINDOW_H
#define WINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QVector>
#include <QPoint>

class Window : public QGraphicsView
{
    Q_OBJECT

public:
    Window();

private slots:
    void solve();
    void generateGraph();
    void exit();
    void restart();

private: // member functions
    void generatePositions();
    void drawEdges();
    void drawVertices();

private:
    QGraphicsScene *m_scene;

    QVector<QPoint> m_vertices;
    int m_numOfVerices;

    // widgets
    QPushButton *startBtn;
    QPushButton *exitBtn;
    QPushButton *generateBtn;
    QPushButton *restartBtn;

};

#endif // WINDOW_H
