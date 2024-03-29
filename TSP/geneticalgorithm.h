#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <QString>
#include <QVector>

#include "tsp.h"

// struct that represents potential solutions
struct Phenotype {
    QVector<int> chromosome;
    int fit;
};

class GeneticAlgorithm : public TSP
{
public:
    GeneticAlgorithm(QString fileName);

    void optimize();

    int minDistance() const;
    QVector<int> bestPath() const;

private: // member functions
    bool stopCondition(int i);
//    int fitness(const QVector<int> &chromo); fitness will be 'evaluatePath' from TSP

    QVector<Phenotype> initialPopulation();
    QVector<Phenotype> selection(const QVector<Phenotype> &population);
    QVector<Phenotype> createGeneration(const QVector<Phenotype> &forReproduction);

    void mutation(QVector<int> &p);
    QPair<QVector<int>, QVector<int>> crossover(const QVector<int> &p1, const QVector<int> &p2);

    QVector<int> createChild(int pos1, int pos2, int size, const QVector<int> &p1, const QVector<int> &p2);
    Phenotype pickOneTournament(const QVector<Phenotype> &pop);

private:
    Phenotype m_best;
    int m_numOfIters;
    int m_populationSize;
    double m_mutationRate;
    int m_tournamentK;
};

#endif // GENETICALGORITHM_H
