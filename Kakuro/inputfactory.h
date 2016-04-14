#ifndef INPUTFACTORY_H
#define INPUTFACTORY_H

#include <QWidget>
#include "kkrboard.h"

QWidget *cellInputFactory(KkrBoard *parent);
void cellInputCleanup();

#endif // INPUTFACTORY_H
