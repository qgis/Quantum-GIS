/***************************************************************************
   qgsvirtualrasterprovider.h - QgsRasterDataProvider
     --------------------------------------
    Date                 : June 10, 2021
    Copyright            : (C) 2021 by Francesco Bursi
    email                : francesco.bursi
***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSVIRTUALRASTERPROVIDER_H
#define QGSVIRTUALRASTERPROVIDER_H

#include <QObject>
#include "qgsrasterdataprovider.h"
#include "qgsprovidermetadata.h"
#include "qgsrastercalculator.h"

class QgsVirtualRasterProvider : public QgsRasterDataProvider
{
    Q_OBJECT
  public:

    QgsVirtualRasterProvider( const QString &uri, const QgsDataProvider::ProviderOptions &providerOptions );
    virtual ~QgsVirtualRasterProvider() override;

    QgsVirtualRasterProvider &operator =( QgsVirtualRasterProvider other ) = delete;
    QString dataSourceUri( bool expandAuthConfig = false ) const override;

    QgsRasterBlock *block( int bandNo, const QgsRectangle &extent, int width, int height, QgsRasterBlockFeedback *feedback = nullptr ) override;
    bool readBlock( int bandNo, QgsRectangle  const &viewExtent, int width, int height, void *data, QgsRasterBlockFeedback *feedback = nullptr ) override
    { Q_UNUSED( bandNo ) Q_UNUSED( viewExtent ); Q_UNUSED( width ); Q_UNUSED( height ); Q_UNUSED( data ); Q_UNUSED( feedback ); return true; }

    // QgsDataProvider interface
    virtual bool isValid() const override;
    virtual QgsCoordinateReferenceSystem crs() const override;
    virtual QgsRectangle extent() const override;
    virtual QString name() const override;
    virtual QString description() const override;

    // QgsRasterInterface interface
    virtual int xBlockSize() const override;
    virtual int yBlockSize() const override;
    virtual int bandCount() const override;
    virtual QgsVirtualRasterProvider *clone() const override;
    virtual Qgis::DataType dataType( int bandNo ) const override;
    virtual Qgis::DataType sourceDataType( int bandNo ) const override;

    int xSize() const override;
    int ySize() const override;

    // QgsRasterDataProvider interface
    virtual QString htmlMetadata() override;
    virtual QString lastErrorTitle() override;
    virtual QString lastError() override;
    int capabilities() const override;

    static QString providerKey();

    QString formulaString();

  private:

    QgsVirtualRasterProvider( const QgsVirtualRasterProvider &other );

    bool mValid = false;
    QgsCoordinateReferenceSystem mCrs;
    QgsRectangle mExtent;
    int mWidth;
    int mHeight;
    int mBandCount = 1;
    int mXBlockSize = 0;
    int mYBlockSize = 0;

    //from qgsrastercalculator
    QString mFormulaString;
    QVector<QgsRasterCalculatorEntry> mRasterEntries;
    QString mLastError;

    std::unique_ptr< QgsRasterCalcNode > mCalcNode;
    QVector <QgsRasterLayer *> mRasterLayers;
};

class QgsVirtualRasterProviderMetadata: public QgsProviderMetadata
{
  public:
    QgsVirtualRasterProviderMetadata();
    QgsVirtualRasterProvider *createProvider( const QString &uri, const QgsDataProvider::ProviderOptions &options, QgsDataProvider::ReadFlags flags = QgsDataProvider::ReadFlags() ) override;
};

#endif // QGSVIRTUALRASTERPROVIDER_H

