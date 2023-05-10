//
//  MapManager+Location.swift
//  Around Town
//
//  Created by Hannan, John Joseph on 10/4/22.
//

import Foundation
import CoreLocation

extension MapManager : CLLocationManagerDelegate {
    
    func locationManagerDidChangeAuthorization(_ manager: CLLocationManager) {
        switch manager.authorizationStatus {
        case .notDetermined:
            locationManager.requestWhenInUseAuthorization()
        case .authorizedAlways, .authorizedWhenInUse:
            locationManager.startUpdatingLocation()
        default:
            locationManager.stopUpdatingLocation()
        }
    }
    

    
    func locationManager(_ manager: CLLocationManager, didUpdateLocations locations: [CLLocation]) {
        let firstLocation = locations[0]
            usersRecentLocation = firstLocation
            region.center = firstLocation.coordinate
    }
    
    func locationManager(_ manager: CLLocationManager, didFailWithError error: Error) {
    }

}
