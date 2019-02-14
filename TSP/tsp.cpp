#include "tsp.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

TSP::TSP(QString fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    if (! file.isOpen()) {
        qDebug() << "opening failed!";
        return ;
    }

    QTextStream in(&file);
    in >> m_numOfVertices;
    m_weights.resize(m_numOfVertices);
    for (int i = 0; i < m_weights.size(); ++i) {
        m_weights[i].resize(m_numOfVertices);
        for (int j = 0; j < m_weights.size(); ++j) {
            in >> m_weights[i][j];
        }
    }

    file.close();
}

int TSP::evaluatePath(const QVector<int> &path)
{
    int distance = 0;
    for (int i = 1; i < path.size(); ++i) {
        distance += m_weights[path[i-1]][path[i]];
    }

    distance += m_weights[path[path.size()-1]][path[0]];

    return distance;
}

TSP::~TSP()
{
    // empty
}
