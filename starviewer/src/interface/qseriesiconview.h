/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/ 
#ifndef UDGSERIESICONVIEW_H
#define UDGSERIESICONVIEW_H

// #include "qseriesiconviewbase.h"
#include "ui_qseriesiconviewbase.h"
#include <QWidget>
#include "series.h"
namespace udg {

/** Aquesta classe �s un widget millorat del Icon View de QT, fet  modificat per poder mostrar les s�ries d'un estudi. 
@author marc
*/

class QSeriesIconView : public QWidget , private Ui::QSeriesIconViewBase{
Q_OBJECT

private :

    QString m_className; 

public:
    QSeriesIconView(QWidget *parent = 0 );

    void insertSeries(Series *serie);
    void clear();
    
    ~QSeriesIconView();

public slots:
    void clearIconView();
    void addSeries(Series *serie);
    void clicked(Q3IconViewItem * item);
    void selectedSeriesList(QString key);
    void view(Q3IconViewItem *item);

signals :
    void selectedSeriesIcon(int index);
    void viewSeriesIcon();
};

};

#endif
