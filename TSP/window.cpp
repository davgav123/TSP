#include <QApplication>
#include <QProcess>
#include <QMessageBox>
#include <QTime>
#include <QString>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QFile>
#include <QTextStream>
#include <QLineF>
#include <QFont>

#include "window.h"
#include "tsp.h"
#include "bruteforce.h"
#include "geneticalgorithm.h"

#include <QDebug>

Window::Window()
{
    // create scene
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 800, 650);

    setScene(m_scene);
    setFixedSize(800, 650);

    m_scene->setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));

    // scroll disabled
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // add buttons
    startBtn = new QPushButton("Start");
    startBtn->resize(100, 50);
    startBtn->move(700, 550);
    startBtn->setDisabled(true);
    startBtn->setStyleSheet("background-color: green; font-weight: bold; font-size: 16px;");
    m_scene->addWidget(startBtn);
    connect(startBtn, SIGNAL(clicked()), this, SLOT(solve()));

    exitBtn = new QPushButton("Exit");
    exitBtn->resize(100, 50);
    exitBtn->move(700, 600);
    exitBtn->setStyleSheet("background-color: red; font-weight: bold; font-size: 16px;");
    m_scene->addWidget(exitBtn);
    connect(exitBtn, SIGNAL(clicked()), this, SLOT(exit()));

    generateBtn = new QPushButton("Generate");
    generateBtn->resize(100, 50);
    generateBtn->move(600, 550);
    generateBtn->setStyleSheet("background-color: orange; font-weight: bold; font-size: 16px;");
    m_scene->addWidget(generateBtn);
    connect(generateBtn, SIGNAL(clicked()), this, SLOT(generateGraph()));

    restartBtn = new QPushButton("Restart");
    restartBtn->resize(100, 50);
    restartBtn->move(600, 600);
    restartBtn->setStyleSheet("background-color: yellow; font-weight: bold; font-size: 16px;");
    m_scene->addWidget(restartBtn);
    connect(restartBtn, SIGNAL(clicked()), this, SLOT(restart()));

    // font for labels
    QFont labelFont;
    labelFont.setPointSize(14);

    // add labels on top
    QGraphicsTextItem *bfLegend = m_scene->addText("Brute force solution");
    bfLegend->setFont(labelFont);
    bfLegend->setPos(10, 5);

    // color for the brute force
    QGraphicsRectItem *bfColor = m_scene->addRect(200, 10, 25, 25);
    bfColor->setBrush(QBrush(Qt::red));

    QGraphicsTextItem *gaLegend = m_scene->addText("Optimization solution");
    gaLegend->setFont(labelFont);
    gaLegend->setPos(250, 5);

    // color for the genetic algorithm
    QGraphicsRectItem *gaColor = m_scene->addRect(460, 10, 25, 25);
    gaColor->setBrush(QBrush(Qt::yellow));

    // add user input
    sbNumOfVertices = new QSpinBox();
    sbNumOfVertices->setRange(6, 12);
    sbNumOfVertices->move(500, 570);
    sbNumOfVertices->setFixedSize(100, 80);
    labelFont.setPointSize(42);
    sbNumOfVertices->setFont(labelFont);
    m_scene->addWidget(sbNumOfVertices);

    // number of nodes
    QGraphicsTextItem *bla = m_scene->addText("Number of nodes");
    QFont selectFont;
    selectFont.setPointSize(8);
    selectFont.setBold(true);
    bla->setFont(selectFont);
    bla->setPos(495, 550);
}

void Window::solve()
{
    qDebug() << "solving...";
    startBtn->setDisabled(true);

    solveBruteForce();

    solveOptimization();
}

void Window::solveOptimization()
{
    GeneticAlgorithm ga("../TSP/adjacencyMatrix.txt");

    QTime gaTimer;
    gaTimer.start();
    ga.optimize();
    int gaElapsedTimeMS = gaTimer.elapsed();

    QVector<int> bestPath = ga.bestPath();
    int bestDist = ga.minDistance();

    // color the path in yellow
    QPen yellow(Qt::yellow, 3);
    drawPath(bestPath, yellow);

    // informations about genetic algorithm solution
    QGraphicsTextItem *txtGenAlg = m_scene->addText(
                "GeneticAlgorithm: shortest path is " + QString::number(bestDist) +
                ". Time: " + QString::number(gaElapsedTimeMS) + " ms");
    txtGenAlg->setPos(10, 600);

    // set font
    QFont f;
    f.setPointSize(13);
    txtGenAlg->setFont(f);
}

void Window::solveBruteForce()
{
    BruteForce bf("../TSP/adjacencyMatrix.txt");
    QTime bfTimer;
    bfTimer.start();
    bf.solve();
    int bfElapsedTimeMS = bfTimer.elapsed();

    QVector<int> bestPath = bf.bestPath();
    int bestDist = bf.minDistance();

    // color the best path in red
    QPen red(Qt::red, 8);
    drawPath(bestPath, red);

    // add info about brute force solution
    QGraphicsTextItem *txtBruteForce = m_scene->addText(
                "BruteForce: shortest path is " + QString::number(bestDist) +
                ". Time: " + QString::number(bfElapsedTimeMS) + " ms");
    txtBruteForce->setPos(10, 570);

    // set font of the brute force info text
    QFont f;
    f.setPointSize(13);
    txtBruteForce->setFont(f);
}


void Window::generateGraph()
{
    qDebug() << "generating...";

    generateBtn->setDisabled(true);
    srand(unsigned(QTime(0, 0, 0).secsTo(QTime::QTime::currentTime())));

    m_numOfVertices = sbNumOfVertices->value();
    m_vertices.resize(m_numOfVertices);
    sbNumOfVertices->setDisabled(true);

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

void Window::restart()
{
    QApplication::quit();
    QProcess::startDetached(QApplication::arguments()[0], QApplication::arguments());
}

void Window::generatePositions()
{
    for (int i = 0; i < m_vertices.size(); ++i) {
        int x = rand() % 700 + 70;
        int y = rand() % 450 + 70;
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
    output << m_numOfVertices << endl;

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

void Window::drawPath(QVector<int> path, QPen pen)
{
    for (int i = 1; i < path.size(); ++i) {
        m_scene->addLine(QLine(m_vertices[path[i-1]], m_vertices[path[i]]), pen);
    }
    m_scene->addLine(QLine(m_vertices[path[path.size()-1]], m_vertices[path[0]]), pen);
}
