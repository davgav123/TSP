#include <QApplication>
#include <QMessageBox>
#include <QTime>
#include <QString>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QFile>
#include <QTextStream>
#include <QLineF>
#include <QPen>

#include "window.h"
#include "bruteforce.h"

#include <QDebug>

Window::Window()
{
    // create scene
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 800, 650);

    setScene(m_scene);
    setFixedSize(800, 650);

    m_scene->setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));
    m_scene->addLine(0, 500, 800, 500);

    // scroll disabled
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // add buttons
    startBtn = new QPushButton("Start");
    startBtn->resize(100, 50);
    startBtn->move(700, 550);
    startBtn->setDisabled(true);
    startBtn->setStyleSheet("background-color: green;");
    m_scene->addWidget(startBtn);
    connect(startBtn, SIGNAL(clicked()), this, SLOT(solve()));

    exitBtn = new QPushButton("Exit");
    exitBtn->resize(100, 50);
    exitBtn->move(700, 600);
    exitBtn->setStyleSheet("background-color: red;");
    m_scene->addWidget(exitBtn);
    connect(exitBtn, SIGNAL(clicked()), this, SLOT(exit()));

    generateBtn = new QPushButton("Generate");
    generateBtn->resize(100, 50);
    generateBtn->move(700, 500);
    generateBtn->setStyleSheet("background-color: yellow;");
    m_scene->addWidget(generateBtn);
    connect(generateBtn, SIGNAL(clicked()), this, SLOT(generateGraph()));

    // the number of vertices will be changed... TODO
    m_numOfVerices = 6;
    m_vertices.resize(m_numOfVerices);
}

void Window::solve()
{
    qDebug() << "solving...";

    // for now we only have brute force
    BruteForce bf("../TSP/adjacencyMatrix.txt");
    bf.solve();

    QVector<int> bestPath = bf.bestPath();
    int bestDist = bf.minDistance();

    qDebug() << "-----------------------------";
    qDebug() << bestPath << " -> " << bestDist;
    qDebug() << "-----------------------------";

    // color the best path in red
    QPen red(Qt::red, 5);
    for (int i = 1; i < bestPath.size(); ++i) {
        m_scene->addLine(QLine(m_vertices[bestPath[i-1]], m_vertices[bestPath[i]]), red);
    }
    m_scene->addLine(QLine(m_vertices[bestPath[bestPath.size()-1]], m_vertices[bestPath[0]]), red);
}

void Window::generateGraph()
{
    qDebug() << "generating...";

    removeGraph();
//    generateBtn->setDisabled(true);
    srand(unsigned(QTime(0, 0, 0).secsTo(QTime::QTime::currentTime())));

    generatePositions();
    drawEdges();
    drawVertices();

    // we can try to solve it now
    startBtn->setDisabled(false);
}

void Window::exit()
{
    qDebug() << "Quit?";

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you sure?", "Quit?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      qDebug() << "Yes!";
      QApplication::quit();
    } else {
      qDebug() << "No!";
    }
}

void Window::generatePositions()
{
    for (int i = 0; i < m_vertices.size(); ++i) {
        int x = rand() % 700 + 50;
        int y = rand() % 400 + 50;
        m_vertices[i] = QPoint(x, y);
    }
}

void Window::drawEdges()
{
    // also, here we will create text file with adjacency matrix
    QFile file(QString("../TSP/adjacencyMatrix.txt"));
    file.open(QFile::WriteOnly);

    if (! file.isOpen()) {
        qDebug() << "failed to open adjacency matrix file...";
        QApplication::exit(); // is exit really needed here??
    }

    QTextStream output(&file);
    output << m_numOfVerices << endl;

    for (int i = 0; i < m_vertices.size(); ++i) {
        for (int j = 0; j < m_vertices.size(); ++j) {
            if (i != j)
                m_scene->addLine(m_vertices[i].x(), m_vertices[i].y(),
                                 m_vertices[j].x(), m_vertices[j].y());

            output << int(qRound(QLineF(m_vertices[i], m_vertices[j]).length() / 10)) << " ";
        }

        output << endl;
    }

    file.close();
}

void Window::drawVertices()
{
    int radius = 30;
    for (int i = 0; i < m_vertices.size(); ++i) {
        QGraphicsEllipseItem *vertex = m_scene->addEllipse(
                    m_vertices[i].x() - radius/2, m_vertices[i].y() - radius/2, radius, radius);
        vertex->setBrush(QBrush(Qt::white));
        QGraphicsTextItem *vertexNum = m_scene->addText(QString::number(i));
        vertexNum->setPos(m_vertices[i].x()-radius/3, m_vertices[i].y()-radius/3);
    }
}

void Window::removeGraph()
{
    // TODO
}
