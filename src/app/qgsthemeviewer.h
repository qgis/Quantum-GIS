#ifndef QGSTHEMEVIEWER_H
#define QGSTHEMEVIEWER_H

#include <QObject>
#include <QWidget>
#include "qgslayertreeview.h"

class QMimeData;

/**
 * QgsThemeViewe class: QgsLayerTreeView with custom drag & drop handling to interact with QgsThemeManagerWidget
 * \since QGIS 3.20
 */

class QgsThemeViewer :  public QgsLayerTreeView
{
    Q_OBJECT
public:
    explicit QgsThemeViewer( QWidget *parent = nullptr );

    //! List supported drop actions
    Qt::DropActions supportedDropActions() const;

signals:

    void layersAdded();

    void layersDropped();

private:

    QStringList mimeTypes() const;

    void dragEnterEvent(QDragEnterEvent *event);

    void dropEvent(QDropEvent *event);

    QMimeData *mimeData() const;

    void startDrag( Qt::DropActions );
};

#endif // QGSTHEMEVIEWER_H
