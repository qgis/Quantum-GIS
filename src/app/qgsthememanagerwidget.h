
#ifndef QGSTHEMEMANAGERWIDGET_H
#define QGSTHEMEMANAGERWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QComboBox>
#include "qgsdockwidget.h"
#include "qgslayertreeview.h"
#include "ui_qgsthememanagerwidgetbase.h"


class QgsMapThemeCollection;
class QMimeData;
class QgsThemeViewer;
#define SIP_NO_FILE


/**
 * QgsThemeManagerWidget class: Used to display layers of selected themes in order to easily modify existing themes.
 * \since QGIS 3.20
 */

class QgsThemeManagerWidget : public QgsDockWidget, private Ui::QgsThemeManagerWidgetBase //QgsLayerTreeProxyModel
{
  Q_OBJECT
  public:

    //! Base constructor
    QgsThemeManagerWidget( QWidget *parent = nullptr );

  signals:
    /**
     * Used to call viewCurrentTheme
     */
    void themeChanged();

    /**
     * Used to add layers from the layertree to the theme
     */
    void addLayerTreeLayers();

    /**
     * Used to remove layers from the theme
     */
    void droppedLayers();

  private slots:

    /**
     * Reset members to match current project.
     */
    void projectLoaded();

    /**
     * Used to catche the ComboBox signal
     */
    void setTheme( const int index );

    /**
     * Triggered by the previous theme button
     */
    void previousTheme();

    /**
     * Triggered by the next theme button
     */
    void nextTheme();

    /**
     * Populate the ComboBox to keep the list synched
     */
    void populateCombo();

    /**
     * Populate the ThemeViewer
     */
    void viewCurrentTheme() const;

    /**
     * Triggered by the all layer button
     */
    void addSelectedLayers();

    /**
     * Triggered by the remove layer button
     */
    void removeSelectedLayers();

    /**
     * Update the comboBox
     */
    void updateComboBox();

    void showWidget();

    void removeTheme();

    void createTheme();

private:


    /**
     * Used by the tool or drag & drop from the main layertree
     */
    void appendLayers( const QList<QgsMapLayer *> &layers );

    /**
     * Used by the tool or drag & drop
     */
    void removeThemeLayers( const QList<QgsMapLayer *> &layers );

    void startDrag( Qt::DropActions );

    QString mCurrentTheme;
    //QToolButton *mThemePrev = nullptr;
    //QToolButton *mThemeNext = nullptr;
    //QToolButton *mAddThemeLayer = nullptr;
    //QToolButton *mRemoveThemeLayer = nullptr;
    //QComboBox *mThemeList = nullptr;
    //QgsLayerTreeView *mThemeViewer = nullptr;
    QgsMapThemeCollection *mThemeCollection = nullptr;

};


#endif
