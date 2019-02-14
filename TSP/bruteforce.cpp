#include "bruteforce.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

BruteForce::BruteForce(QString fileName)
    : TSP(fileName)
{
    // initial path
    for (int i = 0; i < m_numOfVertices; ++i) {
        m_path.push_back(i);
    }
    m_bestPath.resize(m_path.size());
    m_bestPath = m_path;
    m_minDistance = evaluatePath(m_bestPath);
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
