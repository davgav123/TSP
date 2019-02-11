#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include <QString>
#include <QVector>

class BruteForce
{
public:
    BruteForce(QString fileName);

    void solve(int nextIndex = 0);

    int minDistance() const;
    QVector<int> bestPath() const;

private: // member functions
    int evaluatePath(const QVector<int> & path);

private:
    QVector<QVector<int>> m_weights;
    QVector<int> m_path;
    int m_numOfVertices;

    QVector<int> m_bestPath;
    int m_minDistance;
};

#endif // BRUTEFORCE_H
