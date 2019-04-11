#ifndef WINDOW_H
#define WINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QVector>
#include <QPoint>
#include <QPen>
#include <QSpinBox>
#include <QDir>

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
    void drawPath(QVector<int> path, QPen pen);

    void solveBruteForce();
    void solveOptimization();

private:
    QGraphicsScene *m_scene;

    QVector<QPoint> m_vertices;
    int m_numOfVertices;

    QDir m_adjMatrixFilePath;

    // widgets
    QPushButton *startBtn;
    QPushButton *exitBtn;
    QPushButton *generateBtn;
    QPushButton *restartBtn;

    QSpinBox *sbNumOfVertices;

};

#endif // WINDOW_H
