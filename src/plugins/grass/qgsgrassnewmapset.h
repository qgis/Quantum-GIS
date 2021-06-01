/***************************************************************************
    qgsgrassnewmapset.h  - New GRASS Mapset wizard
                             -------------------
    begin                : October, 2005
    copyright            : (C) 2005 by Radim Blazek
    email                : radim.blazek@gmail.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSGRASSNEWMAPSET_H
#define QGSGRASSNEWMAPSET_H

#include "ui_qgsgrassnewmapsetbase.h"
#include "qgscoordinatereferencesystem.h"

class QgsGrassPlugin;
class QgisInterface;
class QgsPointXY;
class QgsProjectionSelectionTreeWidget;

extern "C"
{
#include <grass/gis.h>
}


/**
 * \class QgsGrassNewMapset
 *  \brief GRASS vector edit.
 *
 */
class QgsGrassNewMapset : public QWizard, private Ui::QgsGrassNewMapsetBase
{
    Q_OBJECT

  public:

    enum Page
    {
      Database,
      Location,
      Crs,
      Region,
      MapSet,
      Finish
    };

    //! Constructor
    QgsGrassNewMapset( QgisInterface *iface,
                       QgsGrassPlugin *plugin,
                       QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags() );


    ~QgsGrassNewMapset();

    //! Next page
    int nextId() const override;

    //! Is running
    static bool isRunning();

    //! Close
    void close();

  public slots:
    //! Browse database
    void mDatabaseButton_clicked() { browseDatabase(); }
    void browseDatabase();

    //! Database changed
    void mDatabaseLineEdit_returnPressed() { databaseChanged(); }
    void mDatabaseLineEdit_textChanged() { databaseChanged(); }
    void databaseChanged();

    /***************** LOCATION *****************/
    //! Sets location page
    void setLocationPage();

    //! Sets locations
    void setLocations();

    //! Location radio switched
    void mCreateLocationRadioButton_clicked() { locationRadioSwitched(); }
    void mSelectLocationRadioButton_clicked() { locationRadioSwitched(); }
    void locationRadioSwitched();

    //! Existing location selection
    void mLocationComboBox_textChanged( const QString &txt )
    {
      existingLocationChanged( txt );
    }
    void existingLocationChanged( const QString & );

    //! New location name has changed
    void mLocationLineEdit_returnPressed() { newLocationChanged(); }
    void mLocationLineEdit_textChanged() { newLocationChanged(); }
    void newLocationChanged();

    //! Check location
    void checkLocation();

    /***************** CRS ****************/
    //! Sets projection page, called when entered from location page
    void setProjectionPage();

    //! Projection selected
    void sridSelected();
    void projectionSelected();

    //! Location radio switched
    void mNoProjRadioButton_clicked() { projRadioSwitched(); }
    void mProjRadioButton_clicked() { projRadioSwitched(); }
    void projRadioSwitched();

    //! Sets GRASS projection structures for currently selected projection
    // or CRS_XY if 'not defined' is selected
    void setGrassProjection();

    /******************* REGION ******************/
    //! Sets region page, called when entered from projection
    void setRegionPage();

    //! Sets default GRASS region for current projection
    void setGrassRegionDefaults();

    //! Region Changed
    void mNorthLineEdit_returnPressed() { regionChanged(); }
    void mNorthLineEdit_textChanged() { regionChanged(); }
    void mSouthLineEdit_returnPressed() { regionChanged(); }
    void mSouthLineEdit_textChanged() { regionChanged(); }
    void mEastLineEdit_returnPressed() { regionChanged(); }
    void mEastLineEdit_textChanged() { regionChanged(); }
    void mWestLineEdit_returnPressed() { regionChanged(); }
    void mWestLineEdit_textChanged() { regionChanged(); }
    void regionChanged();

    //! Sets current QGIS region
    void mCurrentRegionButton_clicked() { setCurrentRegion(); }
    void setCurrentRegion();

    //! Sets region selected in combo box
    void mRegionButton_clicked() { setSelectedRegion(); }
    void setSelectedRegion();

    //! Draw current region on map
    void drawRegion();
    void clearRegion();

    /******************* MAPSET *******************/
    //! Sets existing mapsets
    void setMapsets();

    //! Mapset name changed
    void mMapsetLineEdit_returnPressed() { mapsetChanged(); }
    void mMapsetLineEdit_textChanged() { mapsetChanged(); }
    void mapsetChanged();

    /******************** FINISH ******************/
    void mOpenNewMapsetCheckBox_stateChanged( int state );

    //! Sets finish page
    void setFinishPage();

    //! Finish / accept
    void accept() override;

    //! Create new mapset
    void createMapset();

    //! New page was selected
    void pageSelected( int index );

    //! Close event
    void closeEvent( QCloseEvent *e ) override;

    //! Key event
    void keyPressEvent( QKeyEvent *e ) override;

    //! Sets error line
    void setError( QLabel *line, const QString &err = QString() );
  private:
    //! Gets current gisdbase
    QString gisdbase();

    //! Test if current gisdbase directory exists
    bool gisdbaseExists();

    //! Pointer to the QGIS interface object
    QgisInterface *mIface = nullptr;

    //! Plugin
    QgsGrassPlugin *mPlugin = nullptr;

    //! Editing is already running
    static bool sRunning;

    //! Projection selector
    QgsProjectionSelectionTreeWidget *mProjectionSelector = nullptr;

    //! GRASS projection
    struct Cell_head mCellHead;
    struct Key_Value *mProjInfo;
    struct Key_Value *mProjUnits;

    //! Previous page
    int mPreviousPage;

    //! Was the region page modified by user
    bool mRegionModified;

    //! Check region setting
    void checkRegion();

    //! Region map
    QPixmap mPixmap;

    //! Read predefined locations from GML
    void loadRegions();

    //! Locations were initialized
    bool mRegionsInited;

    std::vector<QgsPointXY> mRegionsPoints;

    //! Last projection used for region
    QgsCoordinateReferenceSystem mCrs;
};

#endif // QGSGRASSNEWMAPSET_H
