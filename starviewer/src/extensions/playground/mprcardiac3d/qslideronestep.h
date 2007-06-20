/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQSLIDERONESTEP_H
#define UDGQSLIDERONESTEP_H

#include <QSlider>



namespace udg {


/**
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class QSliderOneStep : public QSlider
{
public:
    QSliderOneStep(QWidget *parent = 0);

    ~QSliderOneStep();

	void wheelEvent ( QWheelEvent * event );

};

}

#endif
