#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include <QString>
#include <QVector>

#include "tsp.h"

class BruteForce : public TSP
{
public:
    BruteForce(QString fileName);

    void solve(int nextIndex = 0);

    int minDistance() const;
    QVector<int> bestPath() const;

private:
    QVector<int> m_path;
    QVector<int> m_bestPath;
    int m_minDistance;
};

#endif // BRUTEFORCE_H
