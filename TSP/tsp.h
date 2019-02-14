#ifndef TSP_H
#define TSP_H

#include <QVector>
#include <QString>

class TSP
{
public:
    TSP(QString fileName);
    virtual ~TSP();

    virtual int minDistance() const = 0;
    virtual QVector<int> bestPath() const = 0;

protected: // member functions
    int evaluatePath(const QVector<int> &path);

protected:
    QVector<QVector<int>> m_weights;
    int m_numOfVertices;
};

#endif // TSP_H
