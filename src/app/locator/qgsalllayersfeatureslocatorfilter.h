/***************************************************************************
                         qgsalllayersfeatureslocatorfilters.h
                         --------------------------
    begin                : May 2017
    copyright            : (C) 2017 by Denis Rouzaud
    email                : denis@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSALLLAYERSFEATURESLOCATORFILTERS_H
#define QGSALLLAYERSFEATURESLOCATORFILTERS_H

#include "qgis_app.h"
#include "qgslocatorfilter.h"
#include "qgsexpression.h"
#include "qgsfeaturerequest.h"
#include "qgsvectorlayerfeatureiterator.h"


class APP_EXPORT QgsAllLayersFeaturesLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    enum ContextMenuEntry
    {
      NoEntry,
      OpenForm
    };

    struct PreparedLayer
    {
      public:
        QgsExpression expression;
        QgsExpressionContext context;
        std::unique_ptr<QgsVectorLayerFeatureSource> featureSource;
        QgsFeatureRequest request;
        QgsFeatureRequest exactMatchRequest;
        QString layerName;
        QString layerId;
        QIcon layerIcon;
    };

    QgsAllLayersFeaturesLocatorFilter( QObject *parent = nullptr );
    QgsAllLayersFeaturesLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "allfeatures" ); }
    QString displayName() const override { return tr( "Features in All Layers" ); }
    Priority priority() const override { return Medium; }
    QString prefix() const override { return QStringLiteral( "af" ); }

    QStringList prepare( const QString &string, const QgsLocatorContext &context ) override;
    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;
    bool hasConfigWidget() const override {return true;}
    void openConfigWidget( QWidget *parent ) override;

  private:
    int mMaxResultsPerLayer = 8;
    int mMaxTotalResults = 15;
    QList<std::shared_ptr<PreparedLayer>> mPreparedLayers;
};

#endif // QGSALLLAYERSFEATURESLOCATORFILTERS_H
