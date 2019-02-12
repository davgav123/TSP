#include "geneticalgorithm.h"

#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <random>

#include <QDebug>

GeneticAlgorithm::GeneticAlgorithm(QString fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    if (! file.isOpen()) {
        qDebug() << "opening failed!";
        return ;
    }

    QTextStream in(&file);
    in >> m_length;
    m_weights.resize(m_length);
    for (int i = 0; i < m_weights.size(); ++i) {
        m_weights[i].resize(m_length);
        for (int j = 0; j < m_weights.size(); ++j) {
            in >> m_weights[i][j];
        }
    }

    file.close();

    // initialize parameters for genetic algorithm
    m_best = {{}, INT_MAX};
    m_populationSize = m_length / 2;
    m_mutationRate = 0.02;

    m_numOfIters = 10;
}

void GeneticAlgorithm::optimize()
{
    QVector<Phenotype> population = initialPopulation();

    int i = 0;
    while (stopCondition(i)) {
        QVector<Phenotype> forReproduction = selection(population);
        population = createGeneration(forReproduction);

        m_best = *std::min_element(population.cbegin(), population.cend(),
                                  [] (Phenotype p1, Phenotype p2) {return p1.fit < p2.fit;});
        i++;
    }

    qDebug() << m_best.chromosome << " " << m_best.fit;
}

int GeneticAlgorithm::minDistance() const
{
    return m_best.fit;
}

QVector<int> GeneticAlgorithm::bestPath() const
{
    return m_best.chromosome;
}

bool GeneticAlgorithm::stopCondition(int i)
{
    if (i < m_numOfIters)
        return true;

    return false;
}

int GeneticAlgorithm::fitness(const QVector<int> &chromo)
{
    int distance = 0;
    for (int i = 1; i < chromo.size(); ++i) {
        distance += m_weights[chromo[i-1]][chromo[i]];
    }
    // add the distance between last and first node
    distance += m_weights[chromo[chromo.size()-1]][chromo[0]];

    return distance;
}

QVector<Phenotype> GeneticAlgorithm::initialPopulation()
{
    QVector<Phenotype> pop(m_populationSize);

    auto rng = std::default_random_engine{};
    for (int i = 0; i < m_populationSize; ++i) {
        pop[i].chromosome.resize(m_length);
        std::iota(pop[i].chromosome.begin(), pop[i].chromosome.end(), 0);
        std::shuffle(pop[i].chromosome.begin(), pop[i].chromosome.end(), rng);

        pop[i].fit = fitness(pop[i].chromosome);
    }

    for (const auto & p : pop)
        qDebug() << p.chromosome << " " << p.fit;

    return pop;
}

QVector<Phenotype> GeneticAlgorithm::selection(const QVector<Phenotype> &population)
{
    // roulette selection TODO
    return population;
}

QVector<Phenotype> GeneticAlgorithm::createGeneration(const QVector<Phenotype> &forReproduction)
{
    return forReproduction;
}
