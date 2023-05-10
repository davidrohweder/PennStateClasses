//
//  Adult_View.swift
//  2D SolarSystem
//
//  Created by David Rohweder on 11/26/22.
//

import SwiftUI

struct Adult_View: View {
    @EnvironmentObject var manager: PlanetManager
    var body: some View {
        VStack {
            Group {
                Text("distanceFromSun \(manager.currentPlanet.distanceFromSun)")
                Text("mass10_24Kg \(manager.currentPlanet.mass10_24Kg)")
                Text("volume10_10Km3 \(manager.currentPlanet.volume10_10Km3)")
                if manager.currentPlanet.equatorialRadius1BarLevelKm != nil {
                    Text("equatorialRadius1BarLevelKm \(manager.currentPlanet.equatorialRadius1BarLevelKm!)")
                }
                if manager.currentPlanet.polarRadiusKm != nil {
                    Text("polarRadius1BarLevelKm \(manager.currentPlanet.polarRadiusKm!)")
                }
                if manager.currentPlanet.polarRadius1BarLevelKm != nil{
                    Text("polarRadius1BarLevelKm \(manager.currentPlanet.polarRadius1BarLevelKm!)")
                }
                Text("volumetricMeanRadiusKm \(manager.currentPlanet.volumetricMeanRadiusKm)")
                if manager.currentPlanet.ellipticityFlattening != nil {
                    Text("ellipticityFlattening \(manager.currentPlanet.ellipticityFlattening!)")
                }
                Text("meanDensityKgM3 \(manager.currentPlanet.meanDensityKgM3)")
                if manager.currentPlanet.gravityEq1BarMS2 != nil {
                    Text("gravityEq1BarMS2 \(manager.currentPlanet.gravityEq1BarMS2!)")
                }
            }
            Group {
                if manager.currentPlanet.accelerationEq1BarMS2 != nil {
                    Text("accelerationEq1BarMS2 \(manager.currentPlanet.accelerationEq1BarMS2!)")
                }
                Text("escapeVelocityKmS \(manager.currentPlanet.escapeVelocityKmS)")
                Text("gMX10_6Km3S2 \(manager.currentPlanet.gMX10_6Km3S2)")
                Text("bondAlbedo  \(manager.currentPlanet.bondAlbedo)")
                Text("geometricAlbedo \(manager.currentPlanet.geometricAlbedo)")
                Text("vBandMagnitudeV10 \(manager.currentPlanet.vBandMagnitudeV10)")
                Text("solarIrradianceWM2 \(manager.currentPlanet.solarIrradianceWM2)")
                Text("blackBodyTemperatureK \(manager.currentPlanet.blackBodyTemperatureK)")
                Text("j2X106 \(manager.currentPlanet.j2X106)")
                Text("numberOfNaturalSatellites \(manager.currentPlanet.numberOfNaturalSatellites)")
            }
            Group {
                Text("planetaryRingSystem " + String(manager.currentPlanet.planetaryRingSystem))
                Text("semimajorAxis10_6Km \(manager.currentPlanet.semimajorAxis10_6Km)")
                Text("siderealOrbitPeriodDays \(manager.currentPlanet.siderealOrbitPeriodDays)")
                Text("tropicalOrbitPeriodDays \(manager.currentPlanet.tropicalOrbitPeriodDays)")
                Text("perihelion10_6Km \(manager.currentPlanet.perihelion10_6Km)")
                Text("aphelion10_6Km \(manager.currentPlanet.aphelion10_6Km)")
                if manager.currentPlanet.synodicPeriodDays != nil {
                    Text("synodicPeriodDays \(manager.currentPlanet.synodicPeriodDays!)")
                }
                Text("meanOrbitalVelocityKmS \(manager.currentPlanet.meanOrbitalVelocityKmS)")
                Text("maxOrbitalVelocityKmS \(manager.currentPlanet.maxOrbitalVelocityKmS)")
                Text("minOrbitalVelocityKmS \(manager.currentPlanet.minOrbitalVelocityKmS)")
            }
            Group {
                Text("orbitInclinationDeg \(manager.currentPlanet.orbitInclinationDeg)")
                Text("orbitEccentricity \(manager.currentPlanet.orbitEccentricity)")
                Text("siderealRotationPeriodHrs \(manager.currentPlanet.siderealRotationPeriodHrs)")
                Text("lengthOfDayHrs \(manager.currentPlanet.lengthOfDayHrs)")
                Text("obliquityToOrbitDeg \(manager.currentPlanet.obliquityToOrbitDeg)")
                if manager.currentPlanet.inclinationOfEquatorDeg != nil {
                    Text("inclinationOfEquatorDeg \(manager.currentPlanet.inclinationOfEquatorDeg!)")
                }
            }
        }
    }
}

struct Adult_View_Previews: PreviewProvider {
    static var previews: some View {
        Adult_View()
    }
}
