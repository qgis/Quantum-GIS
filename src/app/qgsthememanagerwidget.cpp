#include "qgsthememanagerwidget.h"
#include "qgsdockwidget.h"
#include "qgsproject.h"
#include "qgsmaplayer.h"
#include "qgsmapthemecollection.h"
#include "qgslayertreemodel.h"
#include "qgslayertreeview.h"
#include "qgslayertreenode.h"
#include "qgslayertreegroup.h"
#include "qgsthemeviewer.h"
#include "qgsmapthemes.h"
#include <QMessageBox>
#include <QWidget>
#include <QToolButton>
#include <QComboBox>
#include "qgisapp.h"
#include <QDebug>


QgsThemeManagerWidget::QgsThemeManagerWidget( QWidget *parent )
:QgsDockWidget( parent )
{
  qDebug()<<QString("presetup");
  setupUi( this );
  //comboBox.setInsertPolicy( QComboBox::InsertAlphabetically );
  connect( this, &QgsDockWidget::opened, this, &QgsThemeManagerWidget::showWidget );
}

void QgsThemeManagerWidget::projectLoaded()
{
  qDebug()<<QString("proj loaded");
  mThemeCollection = QgsProject::instance()->mapThemeCollection();
  QgsLayerTreeModel *mModel = QgisApp::instance()->layerTreeView()->layerTreeModel();
  qDebug()<<QString("model");
  if ( mModel && mThemeViewer )
  {
    mThemeViewer->setModel( mModel );
    qDebug()<<QString("modlset");
  }
  if ( mThemeCollection && mThemeCollection->mapThemes().length() > 0 )
  {
    mCurrentTheme = mThemeCollection->mapThemes()[0];
    qDebug()<<QString("currentheme");
    //populateCombo();
    mThemeList->setCurrentText( mCurrentTheme );
    populateCombo();
  }
  qDebug()<<QString("proj loadedeededed");
}

void QgsThemeManagerWidget::showWidget()
{
    connect( QgsProject::instance(), &QgsProject::readProject, this, &QgsThemeManagerWidget::projectLoaded );
    connect( mThemePrev, &QToolButton::clicked, this, &QgsThemeManagerWidget::previousTheme );
    connect( mThemeNext, &QToolButton::clicked, this, &QgsThemeManagerWidget::nextTheme );
    connect( mCreateTheme, &QToolButton::clicked, this, &QgsThemeManagerWidget::createTheme );
    connect( mDeleteTheme, &QToolButton::clicked, this, &QgsThemeManagerWidget::removeTheme );
    connect( mAddThemeLayer, &QToolButton::clicked, this, &QgsThemeManagerWidget::addSelectedLayers );
    connect( mRemoveThemeLayer, &QToolButton::clicked, this, &QgsThemeManagerWidget::removeSelectedLayers );
    connect( mThemeList,QOverload<int>::of( &QComboBox::currentIndexChanged ), [=]( int index ){ setTheme( index ); });
    connect( mThemeCollection, &QgsMapThemeCollection::mapThemesChanged, this, &QgsThemeManagerWidget::populateCombo );
    connect( mThemeViewer, &QgsThemeViewer::layersAdded, this, &QgsThemeManagerWidget::addSelectedLayers );
    //connect( mThemeViewer, &QgsThemeViewer::layersDropped, this, &QgsThemeManagerWidget::removeSelectedLayers );
    connect( this, &QgsThemeManagerWidget::droppedLayers, this , &QgsThemeManagerWidget::removeSelectedLayers );
    connect( this, &QgsThemeManagerWidget::addLayerTreeLayers, this , &QgsThemeManagerWidget::addSelectedLayers );
}

void QgsThemeManagerWidget::setTheme( const int index )
{
  qDebug()<<QString::number(index);
  QString themename;
  if ( index > -1 && mThemeCollection->mapThemes().size()>index )
  {
    QString themename = mThemeCollection->mapThemes().at(index);
    if ( !mThemeCollection->hasMapTheme( themename ) )
      return;
    mCurrentTheme = themename;
    emit updateComboBox();
  }
}

void QgsThemeManagerWidget::createTheme()
{
  qDebug()<<QString("NEWTHEME");
 QgsMapThemes::instance()->addPreset();
 populateCombo();
}

void QgsThemeManagerWidget::removeTheme()
{
  int res = QMessageBox::question( QgisApp::instance(), tr( "Remove Theme" ),
                                 tr( "Are you sure you want to remove the existing theme “%1”?" ).arg( mCurrentTheme ),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No );
  if ( res == QMessageBox::Yes )
  {
    mThemeCollection->removeMapTheme( mCurrentTheme );
    populateCombo();
  }
}

void QgsThemeManagerWidget::previousTheme()
{
  qDebug()<<QString("previous");
  QStringList themes = mThemeCollection->mapThemes();
  int idx = themes.indexOf( mCurrentTheme );
  if ( idx > 0 )
  {
    mCurrentTheme = themes.at( idx - 1 );
    emit updateComboBox();
  }
}

void QgsThemeManagerWidget::nextTheme()
{
  qDebug()<<QString("next");
  QStringList themes = mThemeCollection->mapThemes();
  int idx = 1 + themes.indexOf( mCurrentTheme );
  if ( idx < themes.size() )
  {
    mCurrentTheme = themes.at( idx );
    emit updateComboBox();
  }
}

void QgsThemeManagerWidget::populateCombo()
{
  mThemeList->clear();
  qDebug()<<QString("populatin combo with ");
  QStringList themes = mThemeCollection->mapThemes();
  if ( !themes.isEmpty() )
    mThemeList->addItems( themes );
}

void QgsThemeManagerWidget::updateComboBox()
{
  qDebug()<<QString("updatecomb")<<mCurrentTheme;
  mThemeList->setCurrentText( mCurrentTheme );
  viewCurrentTheme();
}

void QgsThemeManagerWidget::viewCurrentTheme() const
{
  if ( !mThemeCollection->hasMapTheme( mCurrentTheme ) )
    return;
  qDebug()<<QString("viewing")<<mCurrentTheme;
  QList<QgsMapLayer *> themeLayers = mThemeCollection->mapThemeVisibleLayers( mCurrentTheme );
  QStringList themeIds = mThemeCollection->mapThemeVisibleLayerIds( mCurrentTheme );
  mThemeViewer->proxyModel()->setApprovedIds( themeIds );
}


void QgsThemeManagerWidget::appendLayers( const QList<QgsMapLayer *> &layers )
{
  if ( !mThemeCollection->hasMapTheme( mCurrentTheme ) )
    return;

  QgsMapThemeCollection::MapThemeRecord theme = mThemeCollection->mapThemeState( mCurrentTheme );
  for ( QgsMapLayer *layer : qgis::as_const( layers ) )
  {
    qDebug()<<layer->name();
    QgsMapThemeCollection::MapThemeLayerRecord newRecord( layer );
    theme.addLayerRecord( newRecord );
  }
  mThemeCollection->update( mCurrentTheme, theme );
  viewCurrentTheme();
}

void QgsThemeManagerWidget::addSelectedLayers()
{
  qDebug()<<QString("add selected");
  const QList<QgsMapLayer *> &selectedLayers = QgisApp::instance()->layerTreeView()->selectedLayers();
  appendLayers( selectedLayers );
}

void QgsThemeManagerWidget::removeSelectedLayers()
{
  qDebug()<<QString("remove selected");
  const QList<QgsMapLayer *> &selectedLayers = mThemeViewer->selectedLayers();
  removeThemeLayers( selectedLayers );
}

void QgsThemeManagerWidget::removeThemeLayers( const QList<QgsMapLayer *> &layers )
{
  if ( !mThemeCollection->hasMapTheme( mCurrentTheme ) )
    return;
  QgsMapThemeCollection::MapThemeRecord theme = mThemeCollection->mapThemeState( mCurrentTheme );
  for ( QgsMapLayer *layer : qgis::as_const( layers ) )
  {
    qDebug()<<layer->name();
    theme.removeLayerRecord( layer );
  }
  mThemeCollection->update( mCurrentTheme, theme );
  viewCurrentTheme();
}


