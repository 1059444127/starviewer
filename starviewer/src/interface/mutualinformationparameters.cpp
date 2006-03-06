/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/

#include "mutualinformationparameters.h"

namespace udg{

MutualInformationParameters::MutualInformationParameters(QObject *parent, const char *name )
 : Parameters(parent,name)
{
    // m�trica
    m_fixedImageStandardDeviation = 0.4;
    m_movingImageStandardDeviation = 0.4;
    m_spatialSamples = 50;
    
    // filtre gaussi�
    m_fixedImageVariance = 2.0; 
    m_movingImageVariance = 2.0;
    
    // optimitzador
    m_learningRate = 0.00001;
    m_numberOfIterations = 50;
}

MutualInformationParameters::~MutualInformationParameters()
{
}

} // end namespace udg
