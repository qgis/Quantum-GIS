#include "qgsthemeviewer.h"
#include "qgsmaplayer.h"
#include <QDebug>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QWidget>
#include <QMimeData>

QgsThemeViewer::QgsThemeViewer( QWidget *parent )
    : QgsLayerTreeView( parent )
{

}


QStringList QgsThemeViewer::mimeTypes() const
{
  qDebug()<<QString("mimetype");
  QStringList types;
  types << QStringLiteral( "application/qgis.thememanagerdata" );
  return types;
}

void QgsThemeViewer::dragEnterEvent(QDragEnterEvent *event) //incom1
{
  qDebug()<<QString("dragenter");
  if (event->mimeData()->hasFormat( QStringLiteral( "application/qgis.layertreemodeldata" ) ) )
      event->acceptProposedAction();
  else if (event->mimeData()->hasFormat( QStringLiteral( "application/qgis.thememanagerdata" ) ) )
    qDebug() << QString("hello self");
}

void QgsThemeViewer::dropEvent(QDropEvent *event) //incom2
{
  qDebug()<<QString("dropevent");
  if (event->mimeData()->hasFormat( QStringLiteral( "application/qgis.layertreemodeldata" ) ) )
    emit layersAdded();
  else if (event->mimeData()->hasFormat( QStringLiteral( "application/qgis.thememanagerdata" ) ) )
    qDebug() << QString("hello self2");
}

Qt::DropActions QgsThemeViewer::supportedDropActions() const
{
  qDebug()<<QString("supporteddrop");
  return Qt::CopyAction | Qt::LinkAction;
}


QMimeData *QgsThemeViewer::mimeData() const //outdrag2
{
  qDebug()<<QString("mimedata");
  QMimeData *mimeData = new QMimeData();
  QList<QgsMapLayer *> layers = selectedLayers();
  //QList<QgsLayerTreeNode *> nodesFinal = indexes2nodes( sortedIndexes, true );

  if ( layers.isEmpty() )
    return mimeData;

  QStringList ids;
  for ( QgsMapLayer *layer : qgis::as_const( layers ) )
  {
    ids << layer->id().toUtf8();
  }

  mimeData->setData( QStringLiteral( "application/qgis.thememanagerdata" ), ids.join( "+" ).toUtf8() );
  mimeData->setData( QStringLiteral( "application/qgis.application.pid" ), QString::number( QCoreApplication::applicationPid() ).toUtf8() );

  return mimeData;
}



void QgsThemeViewer::startDrag( Qt::DropActions )//outdrag1
{
    qDebug()<<QString("startdrag");
    QMimeData *mimeDat = mimeData();
    QDrag *drag = new QDrag( this );
    drag->setMimeData( mimeDat );

    if ( !( drag->target() == this ) )
      emit layersDropped();
    else
      qDebug() << QString("DROPTARGET IS THIS");
}


