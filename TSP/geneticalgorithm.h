#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <QString>
#include <QVector>

// struct that represents potential solutions
struct Phenotype {
    QVector<int> chromosome;
    int fit;
};

class GeneticAlgorithm
{
public:
    GeneticAlgorithm(QString fileName);

    void optimize();

    int minDistance() const;
    QVector<int> bestPath() const;

private: // member functions
    bool stopCondition(int i);
    int fitness(const QVector<int> &chromo);

    QVector<Phenotype> initialPopulation();
    QVector<Phenotype> selection(const QVector<Phenotype> &population);
    QVector<Phenotype> createGeneration(const QVector<Phenotype> &forReproduction);

    void mutation(Phenotype &p);
    QPair<Phenotype, Phenotype> crossover(const Phenotype &p1, const Phenotype &p2);

private:
    QVector<QVector<int>> m_weights;
    int m_length;

    Phenotype m_best;
    int m_numOfIters;
    int m_populationSize;
    double m_mutationRate;

};

#endif // GENETICALGORITHM_H
