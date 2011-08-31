#ifndef UDGOVERLAY_H
#define UDGOVERLAY_H

#include <QString>

namespace udg {

/**
    Classe que encapsula l'objecte Overlay del DICOM.
    Per més informació, consultar apartat C.9 OVERLAYS, PS 3.3.
 */
class Overlay {
public:
    Overlay();
    ~Overlay();

    /// Assigna/obté les files i columnes de l'overlay
    void setRows(unsigned int rows);
    void setColumns(unsigned int columns);
    unsigned int getRows() const;
    unsigned int getColumns() const;

    /// Constants per definir els tipus d'Overlay permesos
    static const QChar GraphicsType;
    static const QChar ROIType;
    
    /// Assigna/obté el tipus d'overlay
    void setType(const QChar &overlayType);
    QChar getType() const;

    /// Assigna/obté l'origen de l'Overlay
    void setDICOMFormattedOrigin(const QString &origin);
    int getXOrigin() const;
    int getYOrigin() const;

private:
    /// Files i columnes de l'overlay
    unsigned int m_rows, m_columns;

    /// Tipus d'overlay. Valors permesos: G = Graphics, R = ROI
    QChar m_type;

    /// Localització del primer punt de l'overlay respecte als pixels a la imatge, donat com fila\columna.
    /// El pixel superior esquerre té la coordenada 1\1
    /// Valors de columna majors que 1 indiquen que l'origen del pla d'overlay és a la dreta de l'origen de la imatge. 
    /// Valors de fila majors que 1 indiquen que l'origen del pla d'overlay està per sota de l'origen de la imatge.
    /// Valors per sota de 1 indiquen que l'origen del pla d'overlay està per sobre o a l'esquerra de l'origen de la imatge.
    QString m_origin;
};

}

#endif