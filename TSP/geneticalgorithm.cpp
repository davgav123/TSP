#include "geneticalgorithm.h"

#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <random>

#include <QDebug>

GeneticAlgorithm::GeneticAlgorithm(QString fileName)
    : TSP(fileName)
{
    // initialize parameters for genetic algorithm
    m_populationSize = 36; // should be an even number
    m_mutationRate = 0.05;
    m_tournamentK = m_populationSize / 2;

    m_numOfIters = 850;
}

void GeneticAlgorithm::optimize()
{
    QVector<Phenotype> population = initialPopulation();
    m_best = *std::min_element(population.cbegin(), population.cend(),
                              [] (Phenotype p1, Phenotype p2) {return p1.fit < p2.fit;});

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

QVector<Phenotype> GeneticAlgorithm::initialPopulation()
{
    QVector<Phenotype> pop(m_populationSize);

    auto rng = std::default_random_engine{};
    for (int i = 0; i < m_populationSize; ++i) {
        pop[i].chromosome.resize(m_numOfVertices);
        std::iota(pop[i].chromosome.begin(), pop[i].chromosome.end(), 0);
        std::shuffle(pop[i].chromosome.begin(), pop[i].chromosome.end(), rng);

        pop[i].fit = evaluatePath(pop[i].chromosome);
    }

    return pop;
}

// tournament selection
QVector<Phenotype> GeneticAlgorithm::selection(const QVector<Phenotype> &population)
{
    QVector<Phenotype> forReproduction;
    for(int i = 0; i < population.size(); ++i) {
        forReproduction.push_back(pickOneTournament(population));
    }

    return forReproduction;
}

Phenotype GeneticAlgorithm::pickOneTournament(const QVector<Phenotype> &pop)
{
    Phenotype best{{}, INT_MAX};
    for(int i = 0; i < m_tournamentK; ++i) {
        int pos = rand() % pop.size();
        if(pop[pos].fit < best.fit){
            best = pop[pos];
        }
    }
    return best;
}

QVector<Phenotype> GeneticAlgorithm::createGeneration(const QVector<Phenotype> &forReproduction)
{
    QVector<Phenotype> newPopulation(m_populationSize);
    // elitism, best path will be in next generation
    newPopulation[0] = m_best;
    newPopulation[1] = m_best;
    for (int i = 2; i < m_populationSize; i=i+2) {
        int parentPos1 = rand() % m_populationSize;
        int parentPos2 = rand() % m_populationSize;

        QPair<QVector<int>, QVector<int>> children =
                crossover(forReproduction[parentPos1].chromosome, forReproduction[parentPos2].chromosome);

        mutation(children.first);
        mutation(children.second);

        newPopulation[i] = {children.first, evaluatePath(children.first)};
        newPopulation[i+1] = {children.second, evaluatePath(children.second)};
    }

    return newPopulation;
}

void GeneticAlgorithm::mutation(QVector<int> &p)
{
    double mutate = double(rand()) / RAND_MAX;
    if (mutate < m_mutationRate) {
        int n = p.length();
        int pos1 = rand() % n;
        int pos2 = rand() % n;

        // mutation will just swap two elements
        int tmp = p[pos1];
        p[pos1] = p[pos2];
        p[pos2] = tmp;
    }
}

// ORDER 1 crossover
QPair<QVector<int>, QVector<int> > GeneticAlgorithm::crossover(const QVector<int> &p1, const QVector<int> &p2)
{
    QPair<QVector<int>, QVector<int>> children;

    int n = p1.size();
    int a = rand() % n;
    int b = rand() % n;

    int pos1 = (a <= b) ? a : b;
    int pos2 = (a > b) ? a : b;

    children.first = createChild(pos1, pos2, n, p1, p2);
    children.second = createChild(pos1, pos2, n, p2, p1);

    return children;
}

QVector<int> GeneticAlgorithm::createChild(int pos1, int pos2, int size, const QVector<int> &p1, const QVector<int> &p2)
{
    QVector<int> child(size);
    child.fill(INT_MAX);
    for (int i = pos1; i < pos2; ++i) {
        child[i] = p1[i];
    }

    int input = pos2;
    for (int i = pos2; i < size; ++i) {
        if (! child.contains(p2[i])) {
            child[input] = p2[i];
            input++;
        }
    }

    for (int i = 0; i < pos2; ++i) {
        if (input == size)
            input = 0;

        if (! child.contains(p2[i])) {
            child[input] = p2[i];
            input++;
        }
    }

    return child;
}












