#include "inputfactory.h"
#include "inputdrag.h"

QWidget *cellInputFactory(KkrBoard *parent)
{
    return new InputDrag(parent);
}

void cellInputCleanup()
{
    // TODO
}
