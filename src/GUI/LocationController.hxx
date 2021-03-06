// LocationController.hxx - GUI launcher dialog using Qt5
//
// Written by James Turner, started October 2015.
//
// Copyright (C) 2015 James Turner <zakalawe@mac.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef LOCATION_CONTROLLER_HXX
#define LOCATION_CONTROLLER_HXX

#include <QObjectList>

#include <Navaids/positioned.hxx>
#include <Airports/airports_fwd.hxx>

#include "QtLauncher_fwd.hxx"
#include "LaunchConfig.hxx"
#include "QmlPositioned.hxx"
#include "UnitsModel.hxx"

class NavaidSearchModel;

class LocationController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)

    Q_PROPERTY(NavaidSearchModel* searchModel MEMBER m_searchModel CONSTANT)

    Q_PROPERTY(QList<QObject*> airportRunways READ airportRunways NOTIFY baseLocationChanged)
    Q_PROPERTY(QList<QObject*> airportParkings READ airportParkings NOTIFY baseLocationChanged)

    Q_PROPERTY(bool offsetEnabled READ offsetEnabled WRITE setOffsetEnabled NOTIFY offsetChanged)
    Q_PROPERTY(QuantityValue offsetRadial READ offsetRadial WRITE setOffsetRadial NOTIFY offsetChanged)
    Q_PROPERTY(QuantityValue offsetDistance READ offsetDistance WRITE setOffsetDistance NOTIFY offsetChanged)

    Q_PROPERTY(bool headingEnabled MEMBER m_headingEnabled NOTIFY configChanged)
    Q_PROPERTY(bool speedEnabled MEMBER m_speedEnabled NOTIFY configChanged)
    Q_PROPERTY(bool altitudeEnabled MEMBER m_altitudeEnabled NOTIFY configChanged)

    Q_PROPERTY(QuantityValue heading MEMBER m_heading NOTIFY configChanged)
    Q_PROPERTY(QuantityValue altitude MEMBER m_altitude NOTIFY configChanged)
    Q_PROPERTY(QuantityValue airspeed MEMBER m_airspeed NOTIFY configChanged)
    Q_PROPERTY(bool onFinal READ onFinal WRITE setOnFinal NOTIFY configChanged)

    Q_PROPERTY(bool isAirportLocation READ isAirportLocation NOTIFY baseLocationChanged)
    Q_PROPERTY(bool useActiveRunway READ useActiveRunway WRITE setUseActiveRunway NOTIFY configChanged)
    Q_PROPERTY(bool useAvailableParking READ useAvailableParking WRITE setUseAvailableParking NOTIFY configChanged)

    Q_PROPERTY(bool tuneNAV1 READ tuneNAV1 WRITE setTuneNAV1 NOTIFY configChanged)
    Q_PROPERTY(QmlGeod baseGeod READ baseGeod WRITE setBaseGeod NOTIFY baseLocationChanged)

    Q_PROPERTY(QmlPositioned* base READ baseLocation CONSTANT)
    Q_PROPERTY(QmlPositioned* detail READ detail CONSTANT)
    Q_PROPERTY(bool isBaseLatLon READ isBaseLatLon NOTIFY baseLocationChanged)

    // allow collecting the location properties to be disabled, if the
    // user is setting conflicting ones
    Q_PROPERTY(bool skipFromArgs MEMBER m_skipFromArgs NOTIFY skipFromArgsChanged)
public:
    explicit LocationController(QObject *parent = nullptr);
    ~LocationController();

    void setLaunchConfig(LaunchConfig* config);

    QString description() const;

    void setBaseLocation(FGPositionedRef ref);

    bool shouldStartPaused() const;

    void setLocationProperties();

    void restoreLocation(QVariantMap l);
    QVariantMap saveLocation() const;

    void restoreSearchHistory();

    /// used to automatically select aircraft state
    bool isParkedLocation() const;

    /// used to automatically select aircraft state
    bool isAirborneLocation() const;

    QuantityValue offsetRadial() const;

    QuantityValue offsetDistance() const
    {
        return m_offsetDistance;
    }

    Q_INVOKABLE void setBaseLocation(QmlPositioned* pos);

    Q_INVOKABLE void setDetailLocation(QmlPositioned* pos);

    QmlGeod baseGeod() const;
    void setBaseGeod(QmlGeod geod);

    bool isAirportLocation() const;

    bool offsetEnabled() const
    {
        return m_offsetEnabled;
    }

    bool onFinal() const
    {
        return m_onFinal;
    }

    void setUseActiveRunway(bool b);

    bool useActiveRunway() const
    {
        return m_useActiveRunway;
    }

    Q_INVOKABLE void addToRecent(QmlPositioned* pos);

    QObjectList airportRunways() const;
    QObjectList airportParkings() const;

    Q_INVOKABLE void showHistoryInSearchModel();

    Q_INVOKABLE QmlGeod parseStringAsGeod(QString string) const;

    bool tuneNAV1() const
    {
        return m_tuneNAV1;
    }

    QmlPositioned* detail() const;

    QmlPositioned* baseLocation() const;

    bool useAvailableParking() const
    {
        return m_useAvailableParking;
    }

    bool isBaseLatLon() const
    {
        return m_locationIsLatLon;
    }

    QuantityValue altitude() const
    {
        return m_altitude;
    }
public slots:
    void setOffsetRadial(QuantityValue offsetRadial);

    void setOffsetDistance(QuantityValue offsetNm);

    void setOffsetEnabled(bool offsetEnabled);

    void setOnFinal(bool onFinal);

    void setTuneNAV1(bool tuneNAV1);

    void setUseAvailableParking(bool useAvailableParking);

Q_SIGNALS:
    void descriptionChanged();
    void offsetChanged();
    void baseLocationChanged();
    void configChanged();
    void skipFromArgsChanged();

private Q_SLOTS:
    void onCollectConfig();
    void onRestoreCurrentLocation();
    void onSaveCurrentLocation();
private:

    void onSearchComplete();
    void addToRecent(FGPositionedRef pos);
    void setNavRadioOption();

    void applyPositionOffset();
    void applyAltitude();
    void applyAirspeed();
    void applyOnFinal();

    NavaidSearchModel* m_searchModel = nullptr;

    FGPositionedRef m_location;
    FGAirportRef m_airportLocation; // valid if m_location is an FGAirport
    FGPositionedRef m_detailLocation; // parking stand or runway detail
    bool m_locationIsLatLon = false;
    SGGeod m_geodLocation;

    FGPositionedList m_recentLocations;
    LaunchConfig* m_config = nullptr;
    QmlPositioned* m_detailQml = nullptr;
    QmlPositioned* m_baseQml = nullptr;

    bool m_offsetEnabled = false;
    QuantityValue m_offsetRadial;
    QuantityValue m_offsetDistance;
    QuantityValue m_heading;
    QuantityValue m_altitude;
    QuantityValue m_airspeed;

    QuantityValue m_defaultAirspeed, m_defaultAltitude,
        m_defaultHeading, m_defaultOffsetDistance, m_defaultOffsetRadial;

    bool m_onFinal = false;
    bool m_useActiveRunway = true;
    bool m_tuneNAV1 = false;
    bool m_useAvailableParking;
    bool m_headingEnabled = false;
    bool m_speedEnabled = false;
    bool m_altitudeEnabled = false;
    bool m_skipFromArgs = false;
};

#endif // LOCATION_CONTROLLER_HXX
