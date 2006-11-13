/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gr�fics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQLOGVIEWER_H
#define UDGQLOGVIEWER_H

#include "ui_qlogviewerbase.h"

namespace udg {

/**
	@author Grup de Gr�fics de Girona  ( GGG ) <vismed@ima.udg.es>
*/
class QLogViewer : public QDialog, private Ui::QLogViewerBase
{
Q_OBJECT
public:
    QLogViewer(QWidget *parent = 0);

    ~QLogViewer();

public slots:
    /// Obre un di�leg per guardar el fitxer de log en una altre ubicaci�
    void saveLogFileAs();

private slots:
    /// Crea les connexions entre signals i slots
    void createConnections();

};

}

#endif
