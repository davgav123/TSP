#include "bruteforce.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

BruteForce::BruteForce(QString fileName)
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

    // initial path
    for (int i = 0; i < m_numOfVertices; ++i) {
        m_path.push_back(i);
    }
    m_bestPath.resize(m_path.size());
    m_bestPath = m_path;
    m_minDistance = evaluatePath(m_bestPath);

//    qDebug() << m_weights;
}

void BruteForce::solve(int nextIndex)
{
    // list every permutation
    if (nextIndex == m_path.size()) {
        int currentDist = evaluatePath(m_path);
        if (currentDist < m_minDistance) {
            m_minDistance = currentDist;
            m_bestPath = m_path;
        }

//        qDebug() << m_path << " -> " << currentDist;
        return ;
    }

    for (int i = nextIndex; i < m_path.size(); ++i) {
        std::swap(m_path[i], m_path[nextIndex]);
        solve(nextIndex+1);
        std::swap(m_path[i], m_path[nextIndex]);
    }
}

int BruteForce::minDistance() const
{
    return m_minDistance;
}

QVector<int> BruteForce::bestPath() const
{
    return m_bestPath;
}

int BruteForce::evaluatePath(const QVector<int> &path)
{
    int distance = 0;
    for (int i = 1; i < path.size(); ++i) {
        distance += m_weights[path[i-1]][path[i]];
    }

    distance += m_weights[path[path.size()-1]][path[0]];

    return distance;
}
