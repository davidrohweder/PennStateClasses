//
//  MapManager_Functions.swift
//  Campus
//
//  Created by user224354 on 10/2/22.
//

import Foundation
import MapKit

extension MapManager {
    
    func FitRegionToBuildings() {
        guard usersRecentLocation != nil else {return}
        var upperLeftCorner = CLLocationCoordinate2D(latitude: usersRecentLocation!.coordinate.latitude, longitude: usersRecentLocation!.coordinate.longitude)
        var bottomRightCorner = CLLocationCoordinate2D(latitude: usersRecentLocation!.coordinate.latitude, longitude: usersRecentLocation!.coordinate.longitude)
        var idx: Int = 0;
        
        for building in buildings {
            if building.isSelected == nil {
                continue
            } else if building.isSelected! {
                upperLeftCorner.latitude = fmax(upperLeftCorner.latitude, building.latitude)
                upperLeftCorner.longitude = fmin(upperLeftCorner.longitude, building.longitude)
                bottomRightCorner.latitude = fmin(bottomRightCorner.latitude, building.latitude)
                bottomRightCorner.longitude = fmax(bottomRightCorner.longitude, building.longitude)
                idx += 1
            }
        }
        guard idx != 0 else {return}
        region.span.latitudeDelta = fabs(upperLeftCorner.latitude - bottomRightCorner.latitude) * 2.5
        region.span.longitudeDelta = fabs(bottomRightCorner.longitude - upperLeftCorner.longitude) * 2.5
    }
    
    func directionsFor(to place:MKPlacemark) {
        genBearingAngle(point: place.coordinate)
        self.routes.removeAll()
        self.isTraveling = true
        let request = MKDirections.Request()
        request.source = MKMapItem.forCurrentLocation()
        request.destination = MKMapItem(placemark: place)
        request.transportType = .walking
        request.requestsAlternateRoutes = true
        
        let directions = MKDirections(request: request)
        directions.calculate { response, error in
            guard error == nil else {return}
            if let route = response?.routes.first {
                self.routes.append(route.polyline)
                self.travelTime = self.formatTravelTime(secs: route.expectedTravelTime)
                let metersInMile = 1609.344
                self.travelDistance = route.distance / metersInMile
                self.steps = route.steps
            }
        }
    }
    
    func formatTravelTime(secs: Double) -> String {
        let formatter = DateComponentsFormatter()
        formatter.allowedUnits = [.hour, .minute, .second]
        formatter.unitsStyle = .positional
        return formatter.string(from: TimeInterval(secs))!
    }
    
    func clearExistingRoute() {
        self.isTraveling = false
        self.routes.removeAll()
    }
    
    func clearCustomPins() {
        custannotations.removeAll()
    }
    
    func genBearingAngle(point: CLLocationCoordinate2D) {
        guard usersRecentLocation != nil else {return}
        let userLong = usersRecentLocation!.coordinate.longitude
        let userLat = usersRecentLocation!.coordinate.latitude
        
        let deltaLong = abs(userLong - point.longitude)
        
        let x = cos(point.latitude) * sin(deltaLong)
        let y = ( cos(userLat) * sin(point.latitude) ) - ( sin(userLat) * cos(point.latitude) * cos(deltaLong) )
        
        let rads = atan2(x,y)
        let deg = rads * (180 / Double.pi)
        
        bearingAngle = deg
    }
    
    func setTerrtainType(type: TerrainTypes) {
        terrainType = type
    }
    
    func imageGiven(buildingIndex: Int) -> String {
        guard buildings[buildingIndex].photo != nil else {return ""}
        return buildings[buildingIndex].photo!
    }
    
    func toggleSelected(toggledBuilding: BuildingModel) {
        guard let index = buildings.firstIndex(of: toggledBuilding) else {return}
        buildings[index].isSelected!.toggle()
        FitRegionToBuildings()
    }
    
    func toggleFavorited(toggledBuilding: BuildingModel) {
        guard let index = buildings.firstIndex(of: toggledBuilding) else {return}
        buildings[index].isFavorited!.toggle()
    }
    
    func setSelectedBuilding(selectedBuilding: BuildingModel) {
        guard let index = buildings.firstIndex(of: selectedBuilding) else {return}
        currentBuilding = buildings[index]
    }
    
}
